if TESTING

XYLIBS = @XLIB_LIBS@ \
         @XINERAMA_LIBS@ \
         @XFT_LIBS@ \
         @LIBUSB_LIBS@

AM_CFLAGS = -D_GNU_SOURCE \
            -Wno-unused-variable \
            -Werror \
            -I$(top_srcdir)/lib \
            @CHECK_CFLAGS@

test_suitedir = .
TESTS = core_tests \
		xlib_tests \
        util_tests
test_suite_PROGRAMS = core_tests \
                      xlib_tests \
                      util_tests

core_tests_SOURCES  = core_tests.c
core_tests_LDADD    = @CHECK_LIBS@

xlib_tests_SOURCES  = xlib_tests.c
xlib_tests_LDADD    = @CHECK_LIBS@ \
                      $(top_builddir)/lib/libxy.a \
                      $(XYLIBS)

util_tests_SOURCES  = util_tests.c
util_tests_LDADD    = @CHECK_LIBS@ \
                      $(top_builddir)/lib/libxy.a \
                      $(XYLIBS)

endif
