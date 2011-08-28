AM_CFLAGS = -D_GNU_SOURCE -Wno-unused-variable -Werror
AM_LDFLAGS = -Wl,--as-needed

XYLIBS = @XLIB_LIBS@ \
		 @XINERAMA_LIBS@ \
		 @XFT_LIBS@ \
		 @LIBUSB_LIBS@

noinst_LIBRARIES = libxy.a
#libxy_a_LIBADD = $(XYLIBS)
libxy_a_SOURCES = broadcast.c \
				  configuration.c \
				  die.c \
				  event.c \
				  ipc.c \
				  lifecycle.c \
				  logging.c \
				  monitor.c \
				  osd.c \
				  state.c \
				  sysinfo.c \
				  util.c \
				  xy.c \
				  xyxlib.c

