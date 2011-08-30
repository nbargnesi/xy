if TESTING

XYLIBS = @XLIB_LIBS@ \
		 @XINERAMA_LIBS@ \
		 @XFT_LIBS@ \
		 @LIBUSB_LIBS@

AM_CFLAGS = -D_GNU_SOURCE -Wno-unused-variable -Werror -I../lib
TESTS = test_suite
test_suite_PROGRAMS = test_suite
test_suitedir = .
test_suite_CFLAGS = @CHECK_CFLAGS@ $(AM_CFLAGS)
test_suite_SOURCES = test_suite.c
test_suite_LDADD = @CHECK_LIBS@ \
				   ../lib/libxy.a \
				   $(XYLIBS)

endif
