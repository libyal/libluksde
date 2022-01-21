/*
 * Library to access the Linux Unified Key Setup (LUKS) Disk Encryption format
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

#if !defined( _LIBLUKSDE_H )
#define _LIBLUKSDE_H

#include <libluksde/codepage.h>
#include <libluksde/definitions.h>
#include <libluksde/error.h>
#include <libluksde/extern.h>
#include <libluksde/features.h>
#include <libluksde/types.h>

#include <stdio.h>

#if defined( LIBLUKSDE_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBLUKSDE_EXTERN \
const char *libluksde_get_version(
             void );

/* Returns the access flags for reading
 */
LIBLUKSDE_EXTERN \
int libluksde_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_get_codepage(
     int *codepage,
     libluksde_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_set_codepage(
     int codepage,
     libluksde_error_t **error );

/* Determines if a file contains a LUKS volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_check_volume_signature(
     const char *filename,
     libluksde_error_t **error );

/* Determines if a file contains a LUKS volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_check_volume_signature_wide(
     const wchar_t *filename,
     libluksde_error_t **error );

#if defined( LIBLUKSDE_HAVE_BFIO )

/* Determines if a file contains a LUKS volume signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libluksde_error_t **error );

#endif /* defined( LIBLUKSDE_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBLUKSDE_EXTERN \
void libluksde_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_notify_set_stream(
     FILE *stream,
     libluksde_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_notify_stream_open(
     const char *filename,
     libluksde_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_notify_stream_close(
     libluksde_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBLUKSDE_EXTERN \
void libluksde_error_free(
      libluksde_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_error_fprint(
     libluksde_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_error_sprint(
     libluksde_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_error_backtrace_fprint(
     libluksde_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_error_backtrace_sprint(
     libluksde_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Volume functions
 * ------------------------------------------------------------------------- */

/* Creates a volume
 * Make sure the value value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_initialize(
     libluksde_volume_t **volume,
     libluksde_error_t **error );

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_free(
     libluksde_volume_t **volume,
     libluksde_error_t **error );

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_signal_abort(
     libluksde_volume_t *volume,
     libluksde_error_t **error );

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_open(
     libluksde_volume_t *volume,
     const char *filename,
     int access_flags,
     libluksde_error_t **error );

#if defined( LIBLUKSDE_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_open_wide(
     libluksde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libluksde_error_t **error );

#endif /* defined( LIBLUKSDE_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBLUKSDE_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_open_file_io_handle(
     libluksde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libluksde_error_t **error );

#endif /* defined( LIBLUKSDE_HAVE_BFIO ) */

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_close(
     libluksde_volume_t *volume,
     libluksde_error_t **error );

/* Determines if the volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_is_locked(
     libluksde_volume_t *volume,
     libluksde_error_t **error );

/* Unlocks the volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_unlock(
     libluksde_volume_t *volume,
     libluksde_error_t **error );

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBLUKSDE_EXTERN \
ssize_t libluksde_volume_read_buffer(
         libluksde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libluksde_error_t **error );

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBLUKSDE_EXTERN \
ssize_t libluksde_volume_read_buffer_at_offset(
         libluksde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libluksde_error_t **error );

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
LIBLUKSDE_EXTERN \
off64_t libluksde_volume_seek_offset(
         libluksde_volume_t *volume,
         off64_t offset,
         int whence,
         libluksde_error_t **error );

/* Retrieves the the current offset of the (volume) data
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_get_offset(
     libluksde_volume_t *volume,
     off64_t *offset,
     libluksde_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_get_size(
     libluksde_volume_t *volume,
     size64_t *size,
     libluksde_error_t **error );

/* Retrieves the encryption method
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_get_encryption_method(
     libluksde_volume_t *volume,
     int *encryption_method,
     int *encryption_chaining_mode,
     libluksde_error_t **error );

/* Retrieves the volume identifier
 * The identifier is an UUID and is 16 bytes of size
 * Returns 1 if successful, 0 if not or or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_get_volume_identifier(
     libluksde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libluksde_error_t **error );

/* Sets the key
 * This function needs to be used before one of the open functions
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_set_key(
     libluksde_volume_t *volume,
     const uint8_t *master_key,
     size_t master_key_size,
     libluksde_error_t **error );

/* Sets the keys
 * This function needs to be used before one of the open functions
 *
 * This function deprecated use libluksde_volume_set_key instead
 *
 * Returns 1 if successful or -1 on error
 */
LIBLUKSDE_DEPRECATED \
LIBLUKSDE_EXTERN \
int libluksde_volume_set_keys(
     libluksde_volume_t *volume,
     const uint8_t *master_key,
     size_t master_key_size,
     libluksde_error_t **error );

/* Sets an UTF-8 formatted password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_set_utf8_password(
     libluksde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libluksde_error_t **error );

/* Sets an UTF-16 formatted password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
LIBLUKSDE_EXTERN \
int libluksde_volume_set_utf16_password(
     libluksde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libluksde_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_H ) */
