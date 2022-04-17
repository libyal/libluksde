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
	@LIBLUKSDE_DLL_EXPORT@

lib_LTLIBRARIES = libluksde.la

libluksde_la_SOURCES = \
	libluksde.c \
	libluksde_codepage.h \
	libluksde_debug.c libluksde_debug.h \
	libluksde_definitions.h \
	libluksde_diffuser.c libluksde_diffuser.h \
	libluksde_encryption_context.c libluksde_encryption_context.h \
	libluksde_error.c libluksde_error.h \
	libluksde_extern.h \
	libluksde_io_handle.c libluksde_io_handle.h \
	libluksde_key_slot.c libluksde_key_slot.h \
	libluksde_libbfio.h \
	libluksde_libcaes.h \
	libluksde_libcdata.h \
	libluksde_libcerror.h \
	libluksde_libclocale.h \
	libluksde_libcnotify.h \
	libluksde_libcthreads.h \
	libluksde_libfcache.h \
	libluksde_libfcrypto.h \
	libluksde_libfguid.h \
	libluksde_libhmac.h \
	libluksde_libuna.h \
	libluksde_notify.c libluksde_notify.h \
	libluksde_password.c libluksde_password.h \
	libluksde_sector_data.c libluksde_sector_data.h \
	libluksde_sector_data_vector.c libluksde_sector_data_vector.h \
	libluksde_support.c libluksde_support.h \
	libluksde_types.h \
	libluksde_unused.h \
	libluksde_volume.c libluksde_volume.h \
	libluksde_volume_header.c libluksde_volume_header.h \
	luksde_keyslot.h \
	luksde_volume.h

libluksde_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCAES_LIBADD@ \
	@LIBFCRYPTO_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@PTHREAD_LIBADD@

libluksde_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libluksde_definitions.h.in \
	libluksde.rc \
	libluksde.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libluksde_definitions.h
	/bin/rm -f libluksde.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libluksde ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libluksde_la_SOURCES)

