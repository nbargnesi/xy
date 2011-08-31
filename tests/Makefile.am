if TESTING

XYLIBS = @XLIB_LIBS@ \
         @XINERAMA_LIBS@ \
         @XFT_LIBS@ \
         @LIBUSB_LIBS@

AM_CFLAGS = -D_GNU_SOURCE -Wno-unused-variable -Werror -I../lib @CHECK_CFLAGS@
test_suitedir = .
TESTS = xlib_tests \
        util_tests
test_suite_PROGRAMS = xlib_tests \
                      util_tests

xlib_tests_SOURCES  = xlib_tests.c
xlib_tests_LDADD    = @CHECK_LIBS@ \
                      ../lib/libxy.a \
                      $(XYLIBS)

util_tests_SOURCES  = util_tests.c
util_tests_LDADD    = @CHECK_LIBS@ \
                      ../lib/libxy.a \
                      $(XYLIBS)

endif
