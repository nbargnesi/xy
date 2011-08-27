AM_CFLAGS = -D_GNU_SOURCE -Wno-unused-variable -Werror -I../lib
AM_LDFLAGS = -Wl,--as-needed

XYLIBS = @XLIB_LIBS@ \
		 @XINERAMA_LIBS@ \
		 @XFT_LIBS@ \
		 @LIBUSB_LIBS@

bin_PROGRAMS = xy

xy_SOURCES = main.c

xy_LDADD = ../lib/libxy.a $(XYLIBS)
