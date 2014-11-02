AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBLUKSDE_DLL_IMPORT@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBLUKSDE_DLL_IMPORT@

if HAVE_PYTHON
TESTS_PYLUKSDE  = \
	test_pyluksde_open_close.sh
endif

TESTS = \
	test_open_close.sh

check_SCRIPTS = \
	test_open_close.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	luksde_test_open_close

luksde_test_open_close_SOURCES = \
	luksde_test_libluksde.h \
	luksde_test_libcerror.h \
	luksde_test_libcnotify.h \
	luksde_test_libcstring.h \
	luksde_test_libcsystem.h \
	luksde_test_open_close.c \
	luksde_test_unused.h

luksde_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

