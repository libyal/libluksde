if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBLUKSDE_DLL_IMPORT@

pyexec_LTLIBRARIES = pyluksde.la

pyluksde_la_SOURCES = \
	pyluksde.c pyluksde.h \
	pyluksde_encryption_chaining_modes.c pyluksde_encryption_chaining_modes.h \
	pyluksde_encryption_methods.c pyluksde_encryption_methods.h \
	pyluksde_error.c pyluksde_error.h \
	pyluksde_file_object_io_handle.c pyluksde_file_object_io_handle.h \
	pyluksde_guid.c pyluksde_guid.h \
	pyluksde_hashing_methods.c pyluksde_hashing_methods.h \
	pyluksde_initialization_vector_modes.c pyluksde_initialization_vector_modes.h \
	pyluksde_integer.c pyluksde_integer.h \
	pyluksde_libluksde.h \
	pyluksde_libbfio.h \
	pyluksde_libcerror.h \
	pyluksde_libclocale.h \
	pyluksde_libfguid.h \
	pyluksde_python.h \
	pyluksde_unused.h \
	pyluksde_volume.c pyluksde_volume.h

pyluksde_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFGUID_LIBADD@

pyluksde_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyluksde_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

