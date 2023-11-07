include_HEADERS = \
	libevtx.h

pkginclude_HEADERS = \
	libevtx/codepage.h \
	libevtx/definitions.h \
	libevtx/error.h \
	libevtx/extern.h \
	libevtx/features.h \
	libevtx/types.h

EXTRA_DIST = \
	libevtx.h.in \
	libevtx/definitions.h.in \
	libevtx/features.h.in \
	libevtx/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libevtx.h
	-rm -f libevtx/definitions.h
	-rm -f libevtx/features.h
	-rm -f libevtx/types.h
	-rm -f Makefile

