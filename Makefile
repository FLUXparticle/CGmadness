INCLUDE=-IimageLoader/include -Iglew/include

CC=gcc
CFLAGS=-ansi -pedantic -Wall $(INCLUDE)

CFLAGS += -DGLEW_STATIC -O3

LDFLAGS=
LIBS=-lm -lGLU -lGL
PROJECT=cgmadness
SHADER=golfball spotlight

# Prüfen ob unter Cygwin oder Linux compiliert wird
ifdef COMSPEC
	CFLAGS += -mno-cygwin
	LDFLAGS += -mno-cygwin
	LIBS += -lglut32 -lglu32 -lopengl32
	EXECSUFFIX = .exe
else
	LIBS += -lglut
	EXECSUFFIX = 
endif

EXEC    :=  $(PROJECT)$(EXECSUFFIX)
DEBUG   :=  $(PROJECT).debug$(EXECSUFFIX)
PROFILE :=  $(PROJECT).profile$(EXECSUFFIX)

SRC     :=  $(patsubst ./%,%,$(shell find -name '*.c'))
OBJS    :=  $(SRC:%.c=build/%.o)
DATA    :=  $(wildcard data/*.tga levels/*.lev levels/*.cgm) $(SHADER:%=%.vert %.frag)

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
TAR = $(PROG).tar.bz2
ZIP = $(PROG).zip
CMD = $(PROG).cmd
CLEAN += $(TAR) $(ZIP)

.PHONY: tar
tar: $(TAR)

$(PROG).tar.bz2: $(PROG) $(CMD) $(DATA)
	@echo "  TAR $@"
	@tar cjf $@ $(EXEC) $(CMD) $(DATA)

.PHONY: zip
zip: $(ZIP)

$(PROG).zip: $(PROG) $(CMD) $(DATA)
	@echo "  ZIP $@"
	@zip $@ $(EXEC) $(CMD) $(DATA) > /dev/null

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
