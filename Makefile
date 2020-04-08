HEADERS = \
 code-source.h \
 code-image-set.h \
 code-dream-image.h \
 code-dream-char-info-set.h \
 code-dream-char-info.h \
 code-dream-format-type.h \
 code-dream-code-display-set.h \
 code-dream-code-display.h \
 code-dream-options.h

SOURCES = \
 main.c \
 code-source.c \
 code-image-set.c \
 code-dream-image.c \
 code-dream-char-info-set.c \
 code-dream-char-info.c \
 code-dream-code-display-set.c \
 code-dream-code-display.c

.PHONY: all clean

all: code-dream

clean:
	rm -f $(OBJ_FILES) code-dream

.c.o: $(HEADERS)
	gcc -o $@ -c $<

OBJ_FILES = \
 main.o \
 code-source.o \
 code-image-set.o \
 code-dream-image.o \
 code-dream-char-info-set.o \
 code-dream-char-info.o

code-dream: $(SOURCES) $(HEADERS)
	gcc -o $@ $(SOURCES) -lSDL2 -lSDL2_ttf
