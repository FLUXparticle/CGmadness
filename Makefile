CC := gcc
CFLAGS :=
LDFLAGS :=

BUILD := build

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

SRC     :=  $(wildcard *.c)
MAINS   :=  $(shell ./mains.pl)
DATA    :=  $(wildcard data/*.tga levels/*.lev levels/*.cgm) $(SHADER:%=%.vert) $(SHADER:%=%.frag)
DLL     :=  glut32.dll glew32.dll
LICENSE :=  license.txt
README  :=  README

EXEC    :=  $(MAINS:%.c=%$(EXECSUFFIX))
OBJS    :=  $(SRC:%.c=$(BUILD)/%.o)
DEPS    :=  $(SRC:%=.deps/%.d) $(MAINS:%.c=.deps/%.o.d)
CLEAN   :=  $(OBJS) $(EXEC)

# main part
.PHONY: all
all: $(EXEC)

.PHONY: profile
profile:
	@$(MAKE) BUILD=profile EXECSUFFIX=".profile$(EXECSUFFIX)" CFLAGS="-pg $(CFLAGS)" LDFLAGS="-pg $(LDFLAGS)"

.PHONY: debug
debug:
	@$(MAKE) BUILD=debug EXECSUFFIX=".debug$(EXECSUFFIX)" CFLAGS="-g $(CFLAGS)" LDFLAGS="-g $(LDFLAGS)"

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
CMD := $(PROJECT).cmd
CLEAN += $(TAR) $(SRC_TAR) $(ZIP)

.PHONY: src
src: $(SRC_TAR)

$(SRC_TAR): Makefile $(wildcard *.c *.h) $(DATA) $(LICENSE) $(README)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=cgmadness/%)

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

.deps/%.o.d: %.c modules.pl | .deps/.
	@echo "  MODULES $@"
	@./modules.pl $* > $@

.deps/%.c.d: %.c | .deps/.
	@echo "  DEP $@"
	@$(CC) -MM -MP -MT $@ -MT '$$(BUILD)/$*.o' $(CFLAGS) $< > $@ || rm $@

# create necessary directories
.PRECIOUS: %/.
%/.:
	@echo "  MKDIR $*"
	@mkdir -p $*
