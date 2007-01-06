CC := gcc
CFLAGS := -ansi -pedantic -Wall

CFLAGS += -O3

LDFLAGS :=
LIBS := -lm
PROJECT := cgmadness
SHADER := golfball spotlight

# Prüfen ob unter Cygwin oder Linux compiliert wird
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
DEBUG   :=  $(PROJECT).debug$(EXECSUFFIX)
PROFILE :=  $(PROJECT).profile$(EXECSUFFIX)

SRC     :=  $(wildcard *.c)
OBJS    :=  $(SRC:%.c=build/%.o)
DATA    :=  $(wildcard data/*.tga levels/*.lev levels/*.cgm) $(SHADER:%=%.vert) $(SHADER:%=%.frag)

DEPS    :=  $(SRC:%=.deps/%.d)
CLEAN   :=  $(OBJS) $(EXEC)

.PHONY: all
all: $(EXEC)

.PHONY: debug
debug: $(DEBUG)

.PHONY: profile
profile: $(PROFILE)

# Alle Source-Files compilieren und zusammenlinken
# Expliziete Regel ist notwendig, weil die LIBS sonst
# vor den .o-Files stehen und dadurch die Cygwin-Version
# nicht kompiliert werden kann.
$(EXEC): $(OBJS)
	@echo "  LINK $@"
	@$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

build/%.o: %.c
	@echo "  CC $@"
	@mkdir -p "$(@D)"
	@$(CC) -c $(CFLAGS) $< -o $@

# Profile-Version

$(PROFILE): $(OBJS:%.o=%.profile.o)
	@echo "  LINK (PROFILE) $@"
	@$(CC) $(LDFLAGS) -pg $^ $(LIBS) -o $@

build/%.profile.o: %.c
	@echo "  CC (PROFILE) $@"
	@mkdir -p "$(@D)"
	@$(CC) -c $(CFLAGS) -pg $< -o $@

# Archiv bauen um ausführbares Programm zu verschicken
TAR := $(PROJECT).tar.bz2
SRC_TAR := $(PROJECT)-src.tar.bz2
ZIP := $(PROJECT).zip
CMD := $(PROJECT).cmd
CLEAN += $(TAR) $(ZIP)

.PHONY: src
src: $(SRC_TAR)

$(SRC_TAR): Makefile $(wildcard *.c *.h) .deps build $(DATA) 
	@echo "  TAR $@"
	@tar -C .. --no-recursion -cjf $@ $(^:%=cgmadness/%)

.PHONY: tar
tar: $(TAR)

$(PROJECT).tar.bz2: $(EXEC) $(DATA)
	@echo "  TAR $@"
	@tar -C .. -cjf $@ $(^:%=cgmadness/%)

.PHONY: zip
zip: $(ZIP)

$(PROJECT).zip: $(EXEC) $(CMD) $(DATA)
	@echo "  ZIP $@"
	@zip $@ $^ > /dev/null

# Dokumentation

.PHONY: doc
doc:
	doxygen Doxyfile

# Aufrüumen
.PHONY: clean
clean:
	@echo "  CLEAN"
	@rm -f $(CLEAN)

# Abhängigkeiten automatisch erkennen
include $(DEPS)

.deps/%.c.d: %.c
	@echo "  DEP $@"
	@mkdir -p "$(@D)"
	@( echo -n "$@ " && $(CC) -MM -MP -MT build/$(<:%.cpp=%.o) $(CFLAGS) $< ) > $@ || rm $@
