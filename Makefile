STATIC ?= 0
FLTO   ?= 0
DEBUG  ?= 0
STRIP  ?= 0

# ----------------------------------------------------------------------------
# Flags
# ----------------------------------------------------------------------------

OS_TYPE := $(shell $(CC) -dumpmachine)

ifneq ($(TARGET),)
  CFLAGS += -target $(TARGET)
endif

ifneq ($(SYSROOT),)
  CFLAGS += --sysroot=$(SYSROOT)
endif

ifneq ($(CPU),)
  CFLAGS += -m$(CPU)
endif

CFLAGS += -std=gnu99 -Wall -pedantic -Wno-deprecated-declarations -Ilibnuhash/c99/include -D_FILE_OFFSET_BITS=64

ifneq ($(DEBUG),1)
  CFLAGS += -Ofast -DNDEBUG
else
  CFLAGS += -Og -g
endif

ifeq ($(FLTO),1)
  CFLAGS += -flto
endif

ifneq ($(MARCH),)
  CFLAGS += -march=$(MARCH)
endif

ifneq ($(MTUNE),)
  CFLAGS += -mtune=$(MTUNE)
endif

ifeq ($(STATIC),1)
  LDFLAGS += -static
endif

ifeq ($(STRIP),1)
  LDFLAGS += -Wl,-s
endif

ifneq ($(filter %w64-mingw32 %w64-windows-gnu,$(OS_TYPE)),)
  LDFLAGS += -municode
endif

RCFLAGS += -Ilibnuhash/c99/src

# ----------------------------------------------------------------------------
# Files
# ----------------------------------------------------------------------------

INFILES = $(patsubst %.c,%.o,$(wildcard libnuhash/c99/src/*.c)) $(patsubst %.c,%.o,$(wildcard tool/src/*.c))
OUTFILE = bin/nuhash
OUTPATH = $(patsubst %/,%,$(dir $(OUTFILE)))

ifneq ($(filter %mingw32 %windows-gnu %cygwin %cygnus,$(OS_TYPE)),)
  OUTFILE := $(OUTFILE).exe
endif

ifneq ($(filter %mingw32 %windows-gnu %cygwin %cygnus,$(OS_TYPE)),)
  INFILES += $(patsubst %.rc,%.o,$(wildcard tool/res/*.rc))
endif

# ----------------------------------------------------------------------------
# Rules
# ----------------------------------------------------------------------------

.PHONY: all clean

all: $(OUTFILE)

$(OUTFILE): $(INFILES) | $(OUTPATH)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OUTPATH):
	mkdir -p $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.rc
	windres $(RCFLAGS) -o $@ $<

clean:
	rm -vrf $(OUTPATH)
	find . \! -path '*/.*' -type f -name '*.o' -exec rm -vf {} \;
