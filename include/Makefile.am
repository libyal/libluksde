include_HEADERS = \
	libluksde.h

pkginclude_HEADERS = \
	libluksde/codepage.h \
	libluksde/definitions.h \
	libluksde/error.h \
	libluksde/extern.h \
	libluksde/features.h \
	libluksde/types.h

EXTRA_DIST = \
	libluksde.h.in \
	libluksde/definitions.h.in \
	libluksde/features.h.in \
	libluksde/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libluksde.h
	/bin/rm -f libluksde/definitions.h
	/bin/rm -f libluksde/features.h
	/bin/rm -f libluksde/types.h
	/bin/rm -f Makefile

