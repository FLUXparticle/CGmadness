CC ?= gcc
CFLAGS ?= -O3
LDFLAGS ?=

CFLAGS += -ansi -pedantic -Wall

LIBS := -lm
PROJECT := cgmadness
SHADER := golfball spotlight

# Check if compiling with Linux or Cygwin/MinGW
ifdef COMSPEC
	CFLAGS += -mno-cygwin
	LDFLAGS += -mno-cygwin
	LIBS += -lglut32 -lglu32 -lopengl32 -lglew32
	EXECSUFFIX := .exe
else
	LIBS += -lglut -lGLU -lGL -lGLEW
	EXECSUFFIX := 
endif

EXEC    :=  $(PROJECT)$(EXECSUFFIX)

SRC     :=  $(wildcard *.c)
OBJS    :=  $(SRC:%.c=build/%.o)
DATA    :=  $(wildcard data/*.tga levels/*.lev levels/*.cgm) $(SHADER:%=%.vert) $(SHADER:%=%.frag)
DLL     :=  glut32.dll glew32.dll
LICENSE :=  license.txt

NAME    := $$Name$$
ifneq ($(filter-out release-%,$(NAME)),$(strip $(NAME)))
	VERSIONSUFFIX := -$(subst -,.,$(patsubst release-%,%,$(word 2,$(NAME))))
else
	VERSIONSUFFIX :=
endif

DEPS    :=  $(SRC:%=.deps/%.d)
CLEAN   :=  $(OBJS) $(EXEC)

# main part
.PHONY: all
all: $(EXEC)

$(EXEC): $(OBJS)
	@echo "  LINK $@"
	@$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

build/%.o: %.c
	@echo "  CC $@"
	@mkdir -p "$(@D)"
	@$(CC) -c $(CFLAGS) $< -o $@

# building archives
TAR := $(PROJECT)$(VERSIONSUFFIX).tar.bz2
SRC_TAR := $(PROJECT)$(VERSIONSUFFIX)-src.tar.bz2
ZIP := $(PROJECT)$(VERSIONSUFFIX).zip
CMD := $(PROJECT).cmd
CLEAN += $(TAR) $(SRC_TAR) $(ZIP)

.PHONY: src
src: $(SRC_TAR)

$(SRC_TAR): Makefile $(wildcard *.c *.h) .deps build $(DATA) $(LICENSE)
	@echo "  TAR $@"
	@tar -C .. --no-recursion -cjf $@ $(^:%=cgmadness/%)

.PHONY: tar
tar: $(TAR)

$(TAR): $(EXEC) $(DATA) $(LICENSE)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=cgmadness/%)

.PHONY: zip
zip: $(ZIP)

$(ZIP): $(EXEC) $(CMD) $(DATA) $(DLL) $(LICENSE)
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

.deps/%.c.d: %.c
	@echo "  DEP $@"
	@mkdir -p "$(@D)"
	@( echo -n "$@ " && $(CC) -MM -MP -MT build/$(<:%.cpp=%.o) $(CFLAGS) $< ) > $@ || rm $@
