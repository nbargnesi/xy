EXTRA_DIST = COPYING \
			 README \
			 debug

SUBDIRS = docs lib src tests .
DISTCHECK_CONFIGURE_FLAGS = "--disable-tests"

mc: maintainer-clean

if TESTING
all: check
endif

clean-local:
	@rm -vf xy-*tar*
