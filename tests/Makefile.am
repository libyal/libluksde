AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
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
	@LIBCSYSTEM_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBLUKSDE_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYLUKSDE = \
	test_python_functions.sh
endif

TESTS = \
	test_api_functions.sh \
	test_luksdeinfo.sh \
	$(TESTS_PYLUKSDE)

check_SCRIPTS = \
	pyluksde_test_get_version.py \
	pyluksde_test_open_close.py \
	pyluksde_test_read.py \
	pyluksde_test_seek.py \
	test_api_functions.sh  \
	test_luksdeinfo.sh \
	test_python_functions.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	luksde_test_error \
	luksde_test_open_close \
	luksde_test_read \
	luksde_test_seek \
	luksde_test_support

luksde_test_error_SOURCES = \
	luksde_test_error.c \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_unused.h

luksde_test_error_LDADD = \
	../libluksde/libluksde.la

luksde_test_open_close_SOURCES = \
	luksde_test_libluksde.h \
	luksde_test_libcerror.h \
	luksde_test_libcsystem.h \
	luksde_test_libcthreads.h \
	luksde_test_open_close.c \
	luksde_test_unused.h

luksde_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

luksde_test_read_SOURCES = \
	luksde_test_libcerror.h \
	luksde_test_libcthreads.h \
	luksde_test_libcsystem.h \
	luksde_test_libluksde.h \
	luksde_test_read.c \
	luksde_test_unused.h

luksde_test_read_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

luksde_test_seek_SOURCES = \
	luksde_test_libcerror.h \
	luksde_test_libcsystem.h \
	luksde_test_libcthreads.h \
	luksde_test_libluksde.h \
	luksde_test_seek.c \
	luksde_test_unused.h

luksde_test_seek_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

luksde_test_support_SOURCES = \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_support.c \
	luksde_test_unused.h

luksde_test_support_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

