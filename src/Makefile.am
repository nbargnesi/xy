AM_CFLAGS = -D_GNU_SOURCE -Wno-unused-variable -Werror
AM_LDFLAGS = "-Wl,--as-needed"

if HAVE_CHECK
TESTS = tests
INCLUDES = -I$(top_srcdir) -I/usr/include/freetype2 @CHECK_CFLAGS@
else
TESTS =
INCLUDES = -I$(top_srcdir) -I/usr/include/freetype2
endif

bin_PROGRAMS = xy
noinst_PROGRAMS = $(TESTS)

xy_SOURCES = die.c \
			 event.c \
			 state.c \
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
			 ipc.c \
			 main.c

xy_LDADD = @XLIB_LIBS@ \
		   @XINERAMA_LIBS@ \
		   @XFT_LIBS@ \
		   @LIBUSB_LIBS@

tests_SOURCES = tests.c
tests_LDADD = @CHECK_LIBS@
CLEANFILES = check_tests.log
