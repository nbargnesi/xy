if TESTING

XYLIBS = @XLIB_LIBS@ \
         @XINERAMA_LIBS@ \
         @XFT_LIBS@

AM_CFLAGS = -D_GNU_SOURCE \
            -Wno-unused-variable \
            -Werror \
            -I$(top_srcdir)/lib \
            @CHECK_CFLAGS@

test_suitedir = .
TESTS = core_tests \
        util_tests \
        xlib_tests \
        monitor_tests

test_suite_PROGRAMS     = core_tests \
                          util_tests \
                          xlib_tests \
                          monitor_tests

core_tests_SOURCES      = core_tests.c
core_tests_LDADD        = @CHECK_LIBS@

util_tests_SOURCES      = util_tests.c
util_tests_LDADD        = @CHECK_LIBS@ \
                          $(top_builddir)/lib/libxy.a \
                          $(XYLIBS)

xlib_tests_SOURCES      = xlib_tests.c
xlib_tests_LDADD        = @CHECK_LIBS@ \
                          $(top_builddir)/lib/libxy.a \
                          $(XYLIBS)

monitor_tests_SOURCES   = monitor_tests.c
monitor_tests_LDADD     = @CHECK_LIBS@ \
                          $(top_builddir)/lib/libxy.a \
                          $(XYLIBS)

endif
