INCLUDE=-IimageLoader/include -Iglew/include

CC=gcc
CFLAGS=-ansi -pedantic -Wall $(INCLUDE)

CFLAGS += -DGLEW_STATIC -O3

LDFLAGS=
LIBS=-lm -lGLU -lGL
PROG=cgmadness
SHADER=golfball spotlight

# Pr�fen ob unter Cygwin oder Linux compiliert wird
ifdef COMSPEC
	CFLAGS += -mno-cygwin
	LDFLAGS += -mno-cygwin
	LIBS += -lglut32 -lglu32 -lopengl32
	EXEC = $(PROG).exe
else
	LIBS += -lglut
	EXEC = $(PROG)
endif

ifdef DEBUG
	CFLAGS += -g
	LDFLAGS += -g
endif

ifdef PROFILE
	CFLAGS += -pg
	LDFLAGS += -pg
endif

SRC=$(shell find -name '*.c')
DATA=$(wildcard data/*.tga levels/*.lev levels/*.cgm) $(SHADER:%=%.vert) $(SHADER:%=%.frag)
DEPS=$(SRC:%=%.d)
CLEAN=$(SRC:%.c=%.o) $(DEPS) $(EXEC)

.PHONY: all
all: $(PROG)

# Alle Source-Files compilieren und zusammenlinken
# Expliziete Regel ist notwendig, weil die LIBS sonst
# vor den .o-Files stehen und dadurch die Cygwin-Version
# nicht kompiliert werden kann.
$(PROG): $(SRC:%.c=%.o)
	@echo "  LINK $@"
	@$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

%.o: %.c
	@echo "  CC $@"
	@mkdir -p "$(@D)"
	@$(CC) -c $(CFLAGS) $< -o $@

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


# Aufräumen
.PHONY: clean
clean:
	@echo "  CLEAN"
	@rm -f $(CLEAN)

# Abhängigkeiten automatisch erkennen
include $(DEPS)

%.c.d: %.c
	@echo "  DEP $@"
	@mkdir -p "$(@D)"
	@( echo -n "$@ " && $(CC) -MM $(CFLAGS) $< ) > $@ || rm $@
