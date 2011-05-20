AM_CFLAGS = -D_GNU_SOURCE -Wno-unused-variable -Werror
AM_LDFLAGS = "-Wl,--as-needed"

XYLIBS = @XLIB_LIBS@ \
		 @XINERAMA_LIBS@ \
		 @XFT_LIBS@ \
		 @LIBUSB_LIBS@

if HAVE_CHECK
TESTS = test_suite
test_suite_PROGRAMS = test_suite
test_suitedir = .
test_suite_CFLAGS = @CHECK_CFLAGS@
test_suite_SOURCES = test_suite.c

test_suite_LDADD = @CHECK_LIBS@ \
				   xyxlib.o \
				   monitor.o \
				   xinerama.o \
				   $(XYLIBS)
endif

bin_PROGRAMS = xy

xy_SOURCES = die.c \
			 monitor.c \
			 event.c \
			 state.c \
			 lifecycle.c \
			 osd.c \
			 util.c \
			 modules \
			 restart.c \
			 xinerama.c \
			 screen.c \
			 color.c \
			 configuration.c \
			 logging.c \
			 xyxlib.c \
			 sysinfo.c \
			 broadcast.c \
			 ipc.c \
			 wmcheck.c \
			 xy.c \
			 main.c

xy_LDADD = $(XYLIBS)

