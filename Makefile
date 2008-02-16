# CG Madness - a Marble Madness clone
# Copyright (C) 2007  Sven Reinck <sreinck@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

BUILD := build
DEPS  := .deps

CPREFIX :=

CC := $(CPREFIX)gcc
CFLAGS := -Wall -O3

CXX := $(CPREFIX)g++
CXXFLAGS := $(CFLAGS) -ansi -pedantic -include cstdlib -I.

LD := $(CPREFIX)g++
LDFLAGS :=
LIBS := -lm

PERL := perl

PWD := $(notdir $(shell pwd))
PROJECT := cgmadness
SHADER := golfball ballshadow

# Check if compiling with Linux or Cygwin/MinGW
ifdef COMSPEC
	CFLAGS += -mno-cygwin
	CXXFLAGS += -mno-cygwin
	LDFLAGS += -mno-cygwin
	LIBS += -lglut32 -lglu32 -lopengl32 -lglew32
	BUILD = mingw
	EXECSUFFIX := .exe
else
	LIBS += -lglut -lGLU -lGL -lGLEW
	EXECSUFFIX :=
endif

MAINS   :=  $(shell $(PERL) mains.pl)
SRC     :=  $(subst ./,,$(shell find -name '*.c' -or -name '*.cpp'))
DATA    :=  $(wildcard data/*.tga levels/*.cgm) $(SHADER:%=%.vert) $(SHADER:%=%.frag)
DLL     :=  glut32.dll glew32.dll
DEV     :=  mains.pl modules.pl indent.pro
DOC     :=  license.txt AUTHORS
DOC_DEV :=  $(DOC) README

EXEC    :=  $(MAINS:%=%$(EXECSUFFIX))
DEP     :=  $(SRC:%=$(DEPS)/%.d) $(MAINS:%=$(DEPS)/%.o.d)
CLEAN   :=  $(BUILD) $(EXEC)

# main part
.PHONY: all
all: $(EXEC)

.PHONY: release
release: src tar mingw-zip

.PHONY: mingw-%
mingw-%:
	@$(MAKE) $* COMSPEC=1 CPREFIX="i586-mingw32msvc-"

.PHONY: profile
profile:
	@$(MAKE) BUILD=profile EXECSUFFIX=".profile$(EXECSUFFIX)" CFLAGS="-pg $(CFLAGS) -O0" LDFLAGS="-pg $(filter-out -s,$(LDFLAGS))"

.PHONY: debug
debug:
	@$(MAKE) BUILD=debug EXECSUFFIX=".debug$(EXECSUFFIX)" CFLAGS="-g $(CFLAGS) -O0" LDFLAGS="-g $(filter-out -s,$(LDFLAGS))"

.SECONDEXPANSION:

%$(EXECSUFFIX): $(BUILD)/%.o
	@echo "  LINK $@"
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(BUILD)/%.o: %.c | $$(@D)/.
	@echo "  CC $@"
	@$(CC) -c $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.cpp | $$(@D)/.
	@echo "  CXX $@"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# building archives
BASENAME := $(PROJECT)
SRC_TAR := $(BASENAME)-src.tar.bz2
TAR := $(BASENAME).tar.bz2
ZIP := $(BASENAME).zip
CMD := $(wildcard *.cmd)
CLEAN += $(TAR) $(SRC_TAR) $(ZIP)

.PHONY: src
src: $(SRC_TAR)

$(SRC_TAR): Makefile $(wildcard *.c *.h) $(DATA) $(DEV) $(DOC_DEV)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=$(PWD)/%)

.PHONY: tar
tar: $(TAR)

$(TAR): $(EXEC) $(DATA) $(DOC)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=$(PWD)/%)

.PHONY: zip
zip: $(ZIP)

$(ZIP): $(EXEC) $(CMD) $(DATA) $(DLL) $(DOC)
	@echo "  ZIP $@"
	@zip "$@" $^ > /dev/null 2>&1

# documentation
.PHONY: doc
doc:
	doxygen Doxyfile

# clean up
.PHONY: clean
clean:
	@echo "  CLEAN"
	@rm -rf $(CLEAN)

# dependancies
include $(DEP)

.DELETE_ON_ERROR:

$(DEPS)/%.o.d: %.c modules.pl | $$(@D)/.
	@echo "  MODULES $@"
	@$(PERL) modules.pl $< > $@

$(DEPS)/%.o.d: %.cpp modules.pl | $$(@D)/.
	@echo "  MODULES $@"
	@$(PERL) modules.pl $< > $@

$(DEPS)/%.c.d: %.c | $$(@D)/.
	@echo "  DEP $@"
	@$(CC) -MM -MP -MT $@ -MT '$(BUILD)/$*.o' $(CFLAGS) $< -MF $@

$(DEPS)/%.cpp.d: %.cpp | $$(@D)/.
	@echo "  DEP $@"
	@$(CXX) -MM -MP -MT $@ -MT '$(BUILD)/$*.o' $(CXXFLAGS) $< -MF $@

# create necessary directories
.PRECIOUS: %/.
%/.:
	@echo "  MKDIR $*"
	@mkdir -p $*
