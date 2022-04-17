AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
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
	@LIBFUSE_CPPFLAGS@ \
	@LIBLUKSDE_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	luksdeinfo \
	luksdemount

luksdeinfo_SOURCES = \
	info_handle.c info_handle.h \
	luksdeinfo.c \
	luksdetools_getopt.c luksdetools_getopt.h \
	luksdetools_i18n.h \
	luksdetools_input.c luksdetools_input.h \
	luksdetools_libluksde.h \
	luksdetools_libcerror.h \
	luksdetools_libclocale.h \
	luksdetools_libcnotify.h \
	luksdetools_libcpath.h \
	luksdetools_libcsplit.h \
	luksdetools_libfguid.h \
	luksdetools_libuna.h \
	luksdetools_output.c luksdetools_output.h \
	luksdetools_signal.c luksdetools_signal.h \
	luksdetools_unused.h

luksdeinfo_LDADD = \
	@LIBFGUID_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

luksdemount_SOURCES = \
	luksdemount.c \
	luksdetools_getopt.c luksdetools_getopt.h \
	luksdetools_i18n.h \
	luksdetools_input.c luksdetools_input.h \
	luksdetools_libbfio.h \
	luksdetools_libcdata.h \
	luksdetools_libcerror.h \
	luksdetools_libclocale.h \
	luksdetools_libcnotify.h \
	luksdetools_libcsplit.h \
	luksdetools_libluksde.h \
	luksdetools_output.c luksdetools_output.h \
	luksdetools_signal.c luksdetools_signal.h \
	luksdetools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

luksdemount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libluksde/libluksde.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on luksdeinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(luksdeinfo_SOURCES)
	@echo "Running splint on luksdemount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(luksdemount_SOURCES)

