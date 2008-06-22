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

CC := gcc
CFLAGS := -O3
LDFLAGS :=
PERL := perl

BUILD := $(shell $(CC) -dumpmachine)

CFLAGS += -Wall

LIBS := -lm
PROJECT := cgmadness
SHADER := golfball ballshadow

# Check if compiling with Linux or Cygwin/MinGW
ifdef COMSPEC
	CFLAGS += -mno-cygwin
	LDFLAGS += -mno-cygwin
	LIBS += -lglut32 -lglu32 -lopengl32 -lglew32
	EXECSUFFIX := .exe
else
	CFLAGS += -I/opt/local/include
	LIBS += -lglut -lGLU -lGL -lGLEW
	EXECSUFFIX :=
endif

SRC     :=  $(wildcard *.c)
MAINS   :=  $(shell $(PERL) mains.pl)
DATA    :=  $(wildcard data/*.tga levels/*.cgm) $(SHADER:%=%.vert) $(SHADER:%=%.frag)
DLL     :=  glut32.dll glew32.dll
DEV     :=  mains.pl modules.pl indent.pro
DOC     :=  license.txt AUTHORS
DOC_DEV :=  $(DOC) README

EXEC    :=  $(MAINS:%.c=%$(EXECSUFFIX))
OBJS    :=  $(SRC:%.c=$(BUILD)/%.o)
DEPS    :=  $(SRC:%=.deps/%.d) $(MAINS:%.c=.deps/%.o.d)
CLEAN   :=  $(OBJS) $(EXEC)

# main part
.PHONY: all
all: $(EXEC)

.PHONY: profile
profile:
	@$(MAKE) BUILD=profile EXECSUFFIX=".profile$(EXECSUFFIX)" CFLAGS="-pg $(CFLAGS) -O0" LDFLAGS="-pg $(LDFLAGS)"

.PHONY: debug
debug:
	@$(MAKE) BUILD=debug EXECSUFFIX=".debug$(EXECSUFFIX)" CFLAGS="-g $(CFLAGS) -O0" LDFLAGS="-g $(LDFLAGS)"

%$(EXECSUFFIX):
	@echo "  LINK $@"
	@$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

$(BUILD)/%.o: %.c | $(BUILD)/.
	@echo "  CC $@"
	@$(CC) -c $(CFLAGS) $< -o $@

# building archives
TAR := $(PROJECT).tar.bz2
SRC_TAR := $(PROJECT)-src.tar.bz2
ZIP := $(PROJECT).zip
CMD := $(wildcard *.cmd)
CLEAN += $(TAR) $(SRC_TAR) $(ZIP)

.PHONY: src
src: $(SRC_TAR)

$(SRC_TAR): Makefile $(wildcard *.c *.h) $(DATA) $(DEV) $(DOC_DEV)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=cgmadness/%)

.PHONY: tar
tar: $(TAR)

$(TAR): $(EXEC) $(DATA) $(DOC)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=cgmadness/%)

.PHONY: zip
zip: $(ZIP)

$(ZIP): $(EXEC) $(CMD) $(DATA) $(DLL) $(DOC)
	@echo "  ZIP $@"
	@zip $@ $^ > /dev/null

# documentation
.PHONY: doc
doc:
	doxygen Doxyfile

# clean up
.PHONY: clean
clean:
	@echo "  CLEAN"
	@rm -f $(CLEAN)

# dependancies
include $(DEPS)

.deps/%.o.d: %.c modules.pl | .deps/.
	@echo "  MODULES $@"
	@$(PERL) modules.pl $* > $@

.deps/%.c.d: %.c | .deps/.
	@echo "  DEP $@"
	@$(CC) -MM -MP -MT $@ -MT '$$(BUILD)/$*.o' $(CFLAGS) $< > $@ || rm $@

# create necessary directories
.PRECIOUS: %/.
%/.:
	@echo "  MKDIR $*"
	@mkdir -p $*
