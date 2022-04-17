/*
 * Volume functions
 *
 * Copyright (C) 2013-2022, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBLUKSDE_VOLUME_H )
#define _LIBLUKSDE_VOLUME_H

#include <common.h>
#include <types.h>

#include "libluksde_encryption_context.h"
#include "libluksde_extern.h"
#include "libluksde_io_handle.h"
#include "libluksde_libbfio.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcthreads.h"
#include "libluksde_types.h"
#include "libluksde_sector_data_vector.h"
#include "libluksde_volume_header.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libluksde_internal_volume libluksde_internal_volume_t;

struct libluksde_internal_volume
{
	/* The volume header
	 */
	libluksde_volume_header_t *volume_header;

	/* The master key
	 */
	uint8_t master_key[ 64 ];

	/* The master key size
	 */
	uint32_t master_key_size;

	/* Value to indicate the volume master key is set
	 */
	uint8_t volume_master_key_is_set;

	/* The user password
	 */
	uint8_t *user_password;

        /* User password size
	 */
	size_t user_password_size;

	/* Value to indicate the user password is set
	 */
	uint8_t user_password_is_set;

	/* The encryption context
	 */
	libluksde_encryption_context_t *encryption_context;

	/* The current (storage media) offset
	 */
	off64_t current_offset;

	/* The sector data vector
	 */
	libluksde_sector_data_vector_t *sector_data_vector;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libluksde_io_handle_t *io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* Value to indicate if the volume is locked
	 */
	uint8_t is_locked;

#if defined( HAVE_LIBLUKSDE_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBLUKSDE_EXTERN \
int libluksde_volume_initialize(
     libluksde_volume_t **volume,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_free(
     libluksde_volume_t **volume,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_signal_abort(
     libluksde_volume_t *volume,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_open(
     libluksde_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBLUKSDE_EXTERN \
int libluksde_volume_open_wide(
     libluksde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBLUKSDE_EXTERN \
int libluksde_volume_open_file_io_handle(
     libluksde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_close(
     libluksde_volume_t *volume,
     libcerror_error_t **error );

int libluksde_internal_volume_open_read(
     libluksde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libluksde_internal_volume_open_read_keys(
     libluksde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_is_locked(
     libluksde_volume_t *volume,
     libcerror_error_t **error );

int libluksde_internal_volume_unlock(
     libluksde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_unlock(
     libluksde_volume_t *volume,
     libcerror_error_t **error );

ssize_t libluksde_internal_volume_read_buffer_from_file_io_handle(
         libluksde_internal_volume_t *internal_volume,
         libbfio_handle_t *file_io_handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBLUKSDE_EXTERN \
ssize_t libluksde_volume_read_buffer(
         libluksde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBLUKSDE_EXTERN \
ssize_t libluksde_volume_read_buffer_at_offset(
         libluksde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

off64_t libluksde_internal_volume_seek_offset(
         libluksde_internal_volume_t *internal_volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBLUKSDE_EXTERN \
off64_t libluksde_volume_seek_offset(
         libluksde_volume_t *volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_get_offset(
     libluksde_volume_t *volume,
     off64_t *offset,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_get_size(
     libluksde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_get_encryption_method(
     libluksde_volume_t *volume,
     int *encryption_method,
     int *encryption_chaining_mode,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_get_volume_identifier(
     libluksde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_set_key(
     libluksde_volume_t *volume,
     const uint8_t *master_key,
     size_t master_key_size,
     libcerror_error_t **error );

LIBLUKSDE_DEPRECATED \
LIBLUKSDE_EXTERN \
int libluksde_volume_set_keys(
     libluksde_volume_t *volume,
     const uint8_t *master_key,
     size_t master_key_size,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_set_utf8_password(
     libluksde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_set_utf16_password(
     libluksde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_VOLUME_H ) */

