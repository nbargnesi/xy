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

if HAVE_CHECK
TESTS = test_suite
test_suite_PROGRAMS = test_suite
test_suitedir = .
test_suite_CFLAGS = @CHECK_CFLAGS@
test_suite_SOURCES = test_suite.c
test_suite_LDADD = @CHECK_LIBS@ \
				   broadcast.o \
				   configuration.o \
				   die.o \
				   event.o \
				   ipc.o \
				   lifecycle.o \
				   logging.o \
				   monitor.o \
				   osd.o \
				   state.o \
				   sysinfo.o \
				   util.o \
				   xy.o \
				   xyxlib.o \
				   $(XYLIBS)

endif

