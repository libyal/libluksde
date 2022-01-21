/*
 * Info handle
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

#include <common.h>
#include <byte_stream.h>
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "luksdetools_input.h"
#include "luksdetools_libbfio.h"
#include "luksdetools_libcerror.h"
#include "luksdetools_libcsplit.h"
#include "luksdetools_libfguid.h"
#include "luksdetools_libluksde.h"
#include "luksdetools_libuna.h"
#include "info_handle.h"

#if !defined( LIBLUKSDE_HAVE_BFIO )

extern \
int libluksde_volume_open_file_io_handle(
     libluksde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libluksde_error_t **error );

#endif /* !defined( LIBLUKSDE_HAVE_BFIO ) */

#define INFO_HANDLE_NOTIFY_STREAM		stdout

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int luksdetools_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "luksdetools_system_string_copy_from_64_bit_in_decimal";
	size_t string_index                = 0;
	system_character_t character_value = 0;
	uint8_t maximum_string_index       = 20;
	int8_t sign                        = 1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	*value_64bit = 0;

	if( string[ string_index ] == (system_character_t) '-' )
	{
		string_index++;
		maximum_string_index++;

		sign = -1;
	}
	else if( string[ string_index ] == (system_character_t) '+' )
	{
		string_index++;
		maximum_string_index++;
	}
	while( string_index < string_size )
	{
		if( string[ string_index ] == 0 )
		{
			break;
		}
		if( string_index > (size_t) maximum_string_index )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
			 "%s: string too large.",
			 function );

			return( -1 );
		}
		*value_64bit *= 10;

		if( ( string[ string_index ] >= (system_character_t) '0' )
		 && ( string[ string_index ] <= (system_character_t) '9' ) )
		{
			character_value = (system_character_t) ( string[ string_index ] - (system_character_t) '0' );
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: %" PRIc_SYSTEM " at index: %d.",
			 function,
			 string[ string_index ],
			 string_index );

			return( -1 );
		}
		*value_64bit += character_value;

		string_index++;
	}
	if( sign == -1 )
	{
		*value_64bit *= (uint64_t) -1;
	}
	return( 1 );
}

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     int unattended_mode,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		memory_free(
		 *info_handle );

		*info_handle = NULL;

		return( -1 );
	}
	( *info_handle )->notify_stream   = INFO_HANDLE_NOTIFY_STREAM;
	( *info_handle )->unattended_mode = unattended_mode;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->file_io_handle != NULL )
		{
			if( info_handle_close(
			     *info_handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close info handle.",
				 function );

				result = -1;
			}
		}
		if( memory_set(
		     ( *info_handle )->key_data,
		     0,
		     64 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear key data.",
			 function );

			result = -1;
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->volume != NULL )
	{
		if( libluksde_volume_signal_abort(
		     info_handle->volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal volume to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the key
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_key(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function   = "info_handle_set_key";
	size_t string_length    = 0;
	uint32_t base16_variant = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( memory_set(
	     info_handle->key_data,
	     0,
	     64 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key data.",
		 function );

		goto on_error;
	}
	base16_variant = LIBUNA_BASE16_VARIANT_RFC4648;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( _BYTE_STREAM_HOST_IS_ENDIAN_BIG )
	{
		base16_variant |= LIBUNA_BASE16_VARIANT_ENCODING_UTF16_BIG_ENDIAN;
	}
	else
	{
		base16_variant |= LIBUNA_BASE16_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN;
	}
#endif
	if( ( string_length != 32 )
	 || ( string_length != 64 )
	 || ( string_length != 128 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string length.",
		 function );

		goto on_error;
	}
	if( libuna_base16_stream_copy_to_byte_stream(
	     (uint8_t *) string,
	     string_length,
	     info_handle->key_data,
	     string_length / 2,
	     base16_variant,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy key data.",
		 function );

		goto on_error;
	}
	info_handle->key_data_size = string_length / 2;

	return( 1 );

on_error:
	memory_set(
	 info_handle->key_data,
	 0,
	 64 );

	info_handle->key_data_size = 0;

	return( -1 );
}

/* Sets the password
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_password";
	size_t string_length  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	info_handle->user_password        = string;
	info_handle->user_password_length = string_length;

	return( 1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_volume_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( luksdetools_system_string_copy_from_64_bit_in_decimal(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	info_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Opens the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	system_character_t password[ 64 ];

	static char *function  = "info_handle_open";
	size_t filename_length = 0;
	size_t password_length = 0;
	int result             = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - file IO handle value already set.",
		 function );

		return( -1 );
	}
	if( info_handle->volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - volume value already set.",
		 function );

		return( -1 );
	}
	if( libbfio_file_range_initialize(
	     &( info_handle->file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file IO handle.",
		 function );

		goto on_error;
	}
	filename_length = system_string_length(
	                   filename );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     info_handle->file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     info_handle->file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open set file name.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_set(
	     info_handle->file_io_handle,
	     info_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open set volume offset.",
		 function );

		goto on_error;
	}
	if( libluksde_volume_initialize(
	     &( info_handle->volume ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize volume.",
		 function );

		goto on_error;
	}
	if( info_handle->key_data_size != 0 )
	{
		if( libluksde_volume_set_key(
		     info_handle->volume,
		     info_handle->key_data,
		     info_handle->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set key.",
			 function );

			goto on_error;
		}
	}
	if( info_handle->user_password != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libluksde_volume_set_utf16_password(
		     info_handle->volume,
		     (uint16_t *) info_handle->user_password,
		     info_handle->user_password_length,
		     error ) != 1 )
#else
		if( libluksde_volume_set_utf8_password(
		     info_handle->volume,
		     (uint8_t *) info_handle->user_password,
		     info_handle->user_password_length,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set password.",
			 function );

			goto on_error;
		}
	}
	if( libluksde_volume_open_file_io_handle(
	     info_handle->volume,
	     info_handle->file_io_handle,
	     LIBLUKSDE_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume.",
		 function );

		goto on_error;
	}
	result = libluksde_volume_is_locked(
	          info_handle->volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if volume is locked.",
		 function );

		goto on_error;
	}
	else if( ( result != 0 )
	      && ( info_handle->unattended_mode == 0 ) )
	{
		fprintf(
		 stdout,
		 "Volume is locked and a password is needed to unlock it.\n\n" );

		if( luksdetools_prompt_for_password(
		     stdout,
		     "Password",
		     password,
		     64,
		     error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to retrieve password.\n" );

			goto on_error;
		}
		password_length = system_string_length(
		                   password );

		if( password_length > 0 )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libluksde_volume_set_utf16_password(
			     info_handle->volume,
			     (uint16_t *) password,
			     password_length,
			     error ) != 1 )
#else
			if( libluksde_volume_set_utf8_password(
			     info_handle->volume,
			     (uint8_t *) password,
			     password_length,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set password.",
				 function );

				goto on_error;
			}
			memory_set(
			 password,
			 0,
			 64 );
		}
		fprintf(
		 stdout,
		 "\n\n" );

		result = libluksde_volume_unlock(
		          info_handle->volume,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to unlock volume.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stdout,
			 "Unable to unlock volume.\n\n" );
		}
	}
	return( 1 );

on_error:
	if( info_handle->volume != NULL )
	{
		libluksde_volume_free(
		 &( info_handle->volume ),
		 NULL );
	}
	if( info_handle->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( info_handle->file_io_handle ),
		 NULL );
	}
	memory_set(
	 password,
	 0,
	 64 );

	return( -1 );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close";
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libluksde_volume_close(
	     info_handle->volume,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close volume.",
		 function );

		result = -1;
	}
	if( libluksde_volume_free(
	     &( info_handle->volume ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		result = -1;
	}
	if( libbfio_handle_free(
	     &( info_handle->file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Prints a GUID value
 * Returns 1 if successful or -1 on error
 */
int info_handle_guid_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     const uint8_t *guid_data,
     libcerror_error_t **error )
{
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	static char *function       = "info_handle_guid_value_fprint";
	int result                  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     guid_data,
	     16,
	     LIBFGUID_ENDIAN_BIG,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfguid_identifier_copy_to_utf16_string(
		  guid,
		  (uint16_t *) guid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#else
	result = libfguid_identifier_copy_to_utf8_string(
		  guid,
		  (uint8_t *) guid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "%s: %" PRIs_SYSTEM "\n",
	 value_name,
	 guid_string );

	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints the volume information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	uint8_t guid_data[ 16 ];

	static char *function        = "luksdeinfo_volume_info_fprint";
	int encryption_chaining_mode = 0;
	int encryption_method        = 0;
	int result                   = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Linux Unified Key Setup Disk Encryption information:\n" );

	if( libluksde_volume_get_encryption_method(
	     info_handle->volume,
	     &encryption_method,
	     &encryption_chaining_mode,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve encryption method.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tEncryption method\t\t: " );

	switch( encryption_method )
	{
		case LIBLUKSDE_ENCRYPTION_METHOD_AES:
			fprintf(
			 info_handle->notify_stream,
			 "AES" );

			break;

		case LIBLUKSDE_ENCRYPTION_METHOD_ARC4:
			fprintf(
			 info_handle->notify_stream,
			 "ARC4" );

			break;

		case LIBLUKSDE_ENCRYPTION_METHOD_ANUBIS:
			fprintf(
			 info_handle->notify_stream,
			 "Anubis" );

			break;

		case LIBLUKSDE_ENCRYPTION_METHOD_BLOWFISH:
			fprintf(
			 info_handle->notify_stream,
			 "Blowfish" );

			break;

		case LIBLUKSDE_ENCRYPTION_METHOD_CAST5:
			fprintf(
			 info_handle->notify_stream,
			 "Cast5" );

			break;

		case LIBLUKSDE_ENCRYPTION_METHOD_CAST6:
			fprintf(
			 info_handle->notify_stream,
			 "Cast6" );

			break;

		case LIBLUKSDE_ENCRYPTION_METHOD_SERPENT:
			fprintf(
			 info_handle->notify_stream,
			 "Serpent" );

			break;

		case LIBLUKSDE_ENCRYPTION_METHOD_TWOFISH:
			fprintf(
			 info_handle->notify_stream,
			 "Twofish" );

			break;

		default:
			fprintf(
			 info_handle->notify_stream,
			 "Unknown" );

			break;
	}
	switch( encryption_chaining_mode )
	{
		case LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC:
			fprintf(
			 info_handle->notify_stream,
			 "-CBC" );

			break;

		case LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB:
			fprintf(
			 info_handle->notify_stream,
			 "-ECB" );

			break;

		case LIBLUKSDE_ENCRYPTION_CHAINING_MODE_XTS:
			fprintf(
			 info_handle->notify_stream,
			 "-XTS" );

			break;

		default:
			break;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libluksde_volume_get_volume_identifier(
	     info_handle->volume,
	     guid_data,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume identifier.",
		 function );

		return( -1 );
	}
	if( info_handle_guid_value_fprint(
	     info_handle,
	     "\tVolume identifier\t\t",
	     guid_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print GUID value.",
		 function );

		return( -1 );
	}
	result = libluksde_volume_is_locked(
	          info_handle->volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if volume is locked.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tIs locked\n" );
	}
/* TODO add more info */

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );
}

