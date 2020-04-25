AC_INIT([code-dream],[0.1.7],[eric.p.hutchins@gmail.com])
AC_CONFIG_SRCDIR([src/code-dream-options.h])
AC_CONFIG_HEADERS([src/config.h])

AC_PROG_CC

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_PATH_SDL2([2.0.0])

AC_CHECK_LIB([SDL2_ttf],[TTF_OpenFont],[LIBS="$LIBS -lSDL2_ttf"])

AC_CHECK_LIB([avformat],[avformat_alloc_context],[LIBS="$LIBS -lavformat"])

AC_CHECK_LIB([avcodec],[avcodec_alloc_context3],[LIBS="$LIBS -lavcodec"])

AC_CHECK_LIB([swscale],[sws_getContext],[LIBS="$LIBS -lswscale"])

AC_CHECK_LIB([gif],[EGifSpew],[LIBS="$LIBS -lgif"])

AC_PATH_PROG([EMACS], [emacs])
AC_ARG_VAR([EMACS],[location of emacs])

AC_DEFINE_UNQUOTED([EMACS], "$EMACS", [location of emacs])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
