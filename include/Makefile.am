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
	/bin/rm -f libevtx.h
	/bin/rm -f libevtx/definitions.h
	/bin/rm -f libevtx/features.h
	/bin/rm -f libevtx/types.h
	/bin/rm -f Makefile

