AUTOMAKE_OPTIONS = subdir-objects

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
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCAES_CPPFLAGS@ \
	@LIBFCRYPTO_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBLUKSDE_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYLUKSDE = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_luksdeinfo.sh \
	$(TESTS_PYLUKSDE)

check_SCRIPTS = \
	pyluksde_test_support.py \
	pyluksde_test_volume.py \
	test_library.sh \
	test_luksdeinfo.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	luksde_test_diffuser \
	luksde_test_encryption_context \
	luksde_test_error \
	luksde_test_io_handle \
	luksde_test_key_slot \
	luksde_test_notify \
	luksde_test_password \
	luksde_test_sector_data \
	luksde_test_sector_data_vector \
	luksde_test_support \
	luksde_test_tools_info_handle \
	luksde_test_tools_output \
	luksde_test_tools_signal \
	luksde_test_volume \
	luksde_test_volume_header

luksde_test_diffuser_SOURCES = \
	luksde_test_diffuser.c \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_unused.h

luksde_test_diffuser_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_encryption_context_SOURCES = \
	luksde_test_encryption_context.c \
	luksde_test_libluksde.h \
	luksde_test_libcerror.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_unused.h

luksde_test_encryption_context_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_error_SOURCES = \
	luksde_test_error.c \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_unused.h

luksde_test_error_LDADD = \
	../libluksde/libluksde.la

luksde_test_io_handle_SOURCES = \
	luksde_test_io_handle.c \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_unused.h

luksde_test_io_handle_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_key_slot_SOURCES = \
	luksde_test_key_slot.c \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_unused.h

luksde_test_key_slot_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_notify_SOURCES = \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_notify.c \
	luksde_test_unused.h

luksde_test_notify_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_password_SOURCES = \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_password.c \
	luksde_test_unused.h

luksde_test_password_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_sector_data_SOURCES = \
	luksde_test_functions.c luksde_test_functions.h \
	luksde_test_libbfio.h \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_sector_data.c \
	luksde_test_unused.h

luksde_test_sector_data_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_sector_data_vector_SOURCES = \
	luksde_test_functions.c luksde_test_functions.h \
	luksde_test_libbfio.h \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_sector_data_vector.c \
	luksde_test_unused.h

luksde_test_sector_data_vector_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_support_SOURCES = \
	luksde_test_functions.c luksde_test_functions.h \
	luksde_test_getopt.c luksde_test_getopt.h \
	luksde_test_libbfio.h \
	luksde_test_libcerror.h \
	luksde_test_libclocale.h \
	luksde_test_libcnotify.h \
	luksde_test_libluksde.h \
	luksde_test_libuna.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_support.c \
	luksde_test_unused.h

luksde_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_tools_info_handle_SOURCES = \
	../luksdetools/info_handle.c ../luksdetools/info_handle.h \
	../luksdetools/luksdetools_input.c ../luksdetools/luksdetools_input.h \
	luksde_test_libcerror.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_tools_info_handle.c \
	luksde_test_unused.h

luksde_test_tools_info_handle_LDADD = \
	@LIBFGUID_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_tools_output_SOURCES = \
	../luksdetools/luksdetools_output.c ../luksdetools/luksdetools_output.h \
	luksde_test_libcerror.h \
	luksde_test_macros.h \
	luksde_test_tools_output.c \
	luksde_test_unused.h

luksde_test_tools_output_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_tools_signal_SOURCES = \
	../luksdetools/luksdetools_signal.c ../luksdetools/luksdetools_signal.h \
	luksde_test_libcerror.h \
	luksde_test_macros.h \
	luksde_test_tools_signal.c \
	luksde_test_unused.h

luksde_test_tools_signal_LDADD = \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

luksde_test_volume_SOURCES = \
	luksde_test_functions.c luksde_test_functions.h \
	luksde_test_getopt.c luksde_test_getopt.h \
	luksde_test_libcerror.h \
	luksde_test_libclocale.h \
	luksde_test_libcnotify.h \
	luksde_test_libluksde.h \
	luksde_test_libuna.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_unused.h \
	luksde_test_volume.c

luksde_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

luksde_test_volume_header_SOURCES = \
	luksde_test_functions.c luksde_test_functions.h \
	luksde_test_libbfio.h \
	luksde_test_libcerror.h \
	luksde_test_libluksde.h \
	luksde_test_macros.h \
	luksde_test_memory.c luksde_test_memory.h \
	luksde_test_unused.h \
	luksde_test_volume_header.c

luksde_test_volume_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

