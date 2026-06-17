AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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

check_PYTHONdir = $(abs_top_builddir)

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_PYTHON_SCRIPTS = \
	pyluksde_test_support.py \
	pyluksde_test_volume.py

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
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

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

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libluksde.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libluksde

test_inputs_pyluksde.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pyluksde

test_inputs_luksdeinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) luksdeinfo

test_library: \
	package.m4 \
	test_inputs_libluksde.at \
	test_library.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_inputs_pyluksde.at \
	test_python_module.at

test_tools: \
	package.m4 \
	test_inputs_luksdeinfo.at \
	test_macros.at \
	test_tools.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	$(check_PYTHON_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_macros.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

