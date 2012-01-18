AM_CFLAGS = -D_GNU_SOURCE -Wno-unused-variable -Werror
AM_LDFLAGS = -Wl,--as-needed

EXTRA_DIST = inotify.h \
             broadcast.h \
             configuration.h \
             constants.h \
             core.h \
             event.h \
             ipc.h \
             layout.h \
             lifecycle.h \
             logging.h \
             monitor.h \
             state.h \
             sysinfo.h \
             types.h \
             util.h \
             xy.h \
             xyxlib.h

XYLIBS     = @XLIB_LIBS@ \
             @XINERAMA_LIBS@ \
             @XFT_LIBS@

noinst_LIBRARIES    = libxy.a
libxy_a_SOURCES     = inotify.c \
                      broadcast.c \
                      configuration.c \
                      die.c \
                      event.c \
                      ipc.c \
                      layout.c \
                      lifecycle.c \
                      logging.c \
                      monitor.c \
                      state.c \
                      sysinfo.c \
                      util.c \
                      xy.c \
                      xyxlib.c

