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

CPREFIX :=

CC := $(CPREFIX)gcc
CFLAGS := -Wall

CXX := $(CPREFIX)g++
CXXFLAGS = $(CFLAGS) -pedantic -include cstdlib -include Prefix.pch -I.

LD := $(CPREFIX)g++
LDFLAGS :=
LIBS_OPENGL :=

PERL := perl

MACHINE := $(shell $(CC) -dumpmachine)
BUILD := build/$(MACHINE)
DEPS  := .deps

PWD := $(notdir $(shell pwd))
PROJECT := cgmadness
NAME := CG Madness
SHADER := golfball ballshadow

GL_H := <GL/glew.h>
GLU_H := <GL/glew.h>
GLUT_H := <GL/freeglut.h>

# Check if compiling with Linux or Cygwin/MinGW
ifneq ($(findstring mingw32,$(MACHINE))$(findstring cygwin,$(MACHINE)),)
	CFLAGS += -mno-cygwin
	CFLAGS += -DWIN32
	CXXFLAGS += -DGLUT_DISABLE_ATEXIT_HACK
	LDFLAGS += -mno-cygwin
	LIBS_OPENGL += -lglut32 -lglu32 -lopengl32 -lglew32
	EXECSUFFIX := .exe
endif
ifneq ($(findstring linux-gnu,$(MACHINE)),)
	CXXFLAGS += -ansi
	LIBS_OPENGL += -lglut -lGLU -lGL -lGLEW
	EXECSUFFIX :=
endif
ifneq ($(findstring apple-darwin,$(MACHINE)),)
	CFLAGS += -DDARWIN
	CXXFLAGS += -I/opt/local/include
	LIBS_OPENGL += -framework OpenGL -framework GLUT -lGLEW
	EXECSUFFIX :=
	GL_H := <GL/glew.h>
	GLU_H := <GL/glew.h>
	GLUT_H := <GLUT/glut.h>
endif

CXXFLAGS += -DGL_H="$(GL_H)" -DGLU_H="$(GLU_H)" -DGLUT_H="$(GLUT_H)"

findOpenGL  = $(if $(findstring /callback.o,$(1)),$(LIBS_OPENGL))

FIND_LIBS := findOpenGL

MAINS   :=  $(shell $(PERL) mains.pl)
CODE    :=  $(subst ./,,$(shell find . -name '*.c' -or -name '*.cpp'))
INCLUDE :=  $(subst ./,,$(shell find . -name '*.h' -or -name '*.hpp' -or -name '*.inc'))
SRC     :=  $(CODE) $(INCLUDE)
DATA    :=  $(wildcard data/*.tga levels/*.cgm) $(SHADER:%=%.vert) $(SHADER:%=%.frag)
DLL     :=  glut32.dll glew32.dll
DEV     :=  $(wildcard *.pl)
DOC     :=  license.txt credits.txt
DOC_DEV :=  $(DOC) compile.txt

EXEC    :=  $(MAINS:%=%$(EXECSUFFIX))
DEP     :=  $(CODE:%=$(DEPS)/%.d) $(MAINS:%=$(DEPS)/%.o.d)
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
	@$(MAKE) BUILD=profile/$(MACHINE) EXECSUFFIX=".profile$(EXECSUFFIX)" CFLAGS="-pg $(CFLAGS) -O0" LDFLAGS="-pg $(filter-out -s,$(LDFLAGS))"

.PHONY: debug
debug:
	@$(MAKE) BUILD=debug/$(MACHINE) EXECSUFFIX=".debug$(EXECSUFFIX)" CFLAGS="-g $(CFLAGS) -O0" LDFLAGS="-g $(filter-out -s,$(LDFLAGS))"

.SECONDEXPANSION:

%$(EXECSUFFIX): $(BUILD)/%.o
	@echo "  LINK $@"
	@$(LD) $(LDFLAGS) $^ $(foreach find, $(FIND_LIBS), $(call $(find),$^)) -o $@

$(BUILD)/%.o: %.c | $$(@D)/.
	@echo "  CC $@"
	@$(CC) -c $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.cpp Prefix.pch | $$(@D)/.
	@echo "  CXX $@"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# building archives
BASENAME := $(PROJECT)
SRC_TAR := $(BASENAME)-src.tar.bz2
TAR := $(BASENAME).tar.bz2
ZIP := $(BASENAME).zip
APP := $(NAME).app
CMD := $(wildcard *.cmd)
CLEAN += $(TAR) $(SRC_TAR) $(ZIP)

.PHONY: src
src: $(SRC_TAR)

$(SRC_TAR): Makefile $(SRC) $(DATA) $(DEV) $(DOC_DEV)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=$(PWD)/%)

.PHONY: tar
tar: $(TAR)

$(TAR): $(EXEC) $(DATA) $(DOC)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=$(PWD)/%)

.PHONY: zip
zip: $(ZIP)

ifneq ($(findstring apple-darwin,$(MACHINE)),)
$(ZIP): $(EXEC) $(DATA) $(DOC)
	@echo "  APP $@"
	@ant -Dzip="$(ZIP)" -Dapp="$(APP)" -Dexec="$(word 1,$(EXEC))" -Dres="$(DATA) $(DOC)" > /dev/null
else
$(ZIP): $(EXEC) $(CMD) $(DATA) $(DLL) $(DOC)
	@echo "  ZIP $@"
	@zip "$@" $^ > /dev/null 2>&1
endif

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

$(DEPS)/%.c.d: %.c dep.pl | $$(@D)/.
	@echo "  DEP $@"
	@$(PERL) dep.pl $< > $@

$(DEPS)/%.cpp.d: %.cpp dep.pl | $$(@D)/.
	@echo "  DEP $@"
	@$(PERL) dep.pl $< > $@

# create necessary directories
.PRECIOUS: %/.
%/.:
	@echo "  MKDIR $*"
	@mkdir -p $*
