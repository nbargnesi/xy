EXTRA_DIST = COPYING \
			 README \
			 leaktest \
			 memcheck \
			 debug
SUBDIRS = docs lib src tests .

mc: maintainer-clean

if TESTING
all: check
endif

clean-local:
	@rm -vf xy-*tar*
