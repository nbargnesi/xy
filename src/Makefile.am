INCLUDES = -I$(top_srcdir) -I/usr/include/freetype2
AM_CFLAGS = -D_GNU_SOURCE -Werror

AM_LDFLAGS = "-Wl,--as-needed"

bin_PROGRAMS = xy

xy_SOURCES = state.c \
			 lifecycle.c \
			 osd.c \
			 xy.c \
			 util.c \
			 wmcheck.c \
			 restart.c \
			 xinerama.c \
			 screen.c \
			 color.c \
			 display.c \
			 configuration.c \
			 logging.c \
			 xyxlib.c \
			 sysinfo.c \
			 broadcast.c \
			 ipc.c

xy_LDADD = @XLIB_LIBS@ \
		   @XINERAMA_LIBS@ \
		   @XFT_LIBS@ \
		   @LIBUSB_LIBS@

