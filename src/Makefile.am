bin_PROGRAMS = code-dream
code_dream_SOURCES = \
 code-dream-char-info.c \
 code-dream-char-info.h \
 code-dream-char-info-set.c \
 code-dream-char-info-set.h \
 code-dream-code-display.c \
 code-dream-code-display.h \
 code-dream-code-display-set.c \
 code-dream-code-display-set.h \
 code-dream-format-type.h \
 code-dream-gif-writer.c \
 code-dream-gif-writer.h \
 code-dream-image.c \
 code-dream-image.h \
 code-dream-loading-screen.c \
 code-dream-loading-screen.h \
 code-dream-options.h \
 code-dream-theme.c \
 code-dream-theme.h \
 code-image-set.c \
 code-image-set.h \
 code-source.c \
 code-source.h \
 main.c

code_dream_CFLAGS = $(SDL_CFLAGS)
code_dream_LDFLAGS = $(SDL_LIBS)

code_dream_datadir = $(datadir)/code-dream
code_dream_CFLAGS +=  -DDATADIR=\"$(code_dream_datadir)\"

dist_code_dream_data_DATA = \
 ../highlight.el \
 ../theme.el \
 ../DejaVuSansMono.ttf
