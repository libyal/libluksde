/*
 * Mount handle
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "luksdetools_input.h"
#include "luksdetools_libbfio.h"
#include "luksdetools_libcerror.h"
#include "luksdetools_libcpath.h"
#include "luksdetools_libluksde.h"
#include "luksdetools_libuna.h"
#include "mount_file_entry.h"
#include "mount_file_system.h"
#include "mount_handle.h"

#if !defined( LIBLUKSDE_HAVE_BFIO )

extern \
int libluksde_volume_open_file_io_handle(
     libluksde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libluksde_error_t **error );

#endif /* !defined( LIBLUKSDE_HAVE_BFIO ) */

#define MOUNT_HANDLE_NOTIFY_STREAM		stdout

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int mount_handle_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "mount_handle_system_string_copy_from_64_bit_in_decimal";
	system_character_t character_value = 0;
	size_t string_index                = 0;
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

/* Creates a mount handle
 * Make sure the value mount_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int mount_handle_initialize(
     mount_handle_t **mount_handle,
     int unattended_mode,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_initialize";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mount handle value already set.",
		 function );

		return( -1 );
	}
	*mount_handle = memory_allocate_structure(
	                 mount_handle_t );

	if( *mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create mount handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mount_handle,
	     0,
	     sizeof( mount_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear mount handle.",
		 function );

		goto on_error;
	}
	if( mount_file_system_initialize(
	     &( ( *mount_handle )->file_system ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file system.",
		 function );

		goto on_error;
	}
	( *mount_handle )->notify_stream   = MOUNT_HANDLE_NOTIFY_STREAM;
	( *mount_handle )->unattended_mode = unattended_mode;

	return( 1 );

on_error:
	if( *mount_handle != NULL )
	{
		memory_free(
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( -1 );
}

/* Frees a mount handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_free";
	int result            = 1;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		if( ( *mount_handle )->file_io_handle != NULL )
		{
			if( mount_handle_close(
			     *mount_handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close mount handle.",
				 function );

				result = -1;
			}
		}
		if( mount_file_system_free(
		     &( ( *mount_handle )->file_system ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file system.",
			 function );

			result = -1;
		}
		if( memory_set(
		     ( *mount_handle )->key_data,
		     0,
		     16 ) == NULL )
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
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( result );
}

/* Signals the mount handle to abort
 * Returns 1 if successful or -1 on error
 */
int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_signal_abort";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_file_system_signal_abort(
	     mount_handle->file_system,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal file system to abort.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the key
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_key(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function   = "mount_handle_set_key";
	size_t string_length    = 0;
	uint32_t base16_variant = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( memory_set(
	     mount_handle->key_data,
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
	     mount_handle->key_data,
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
	mount_handle->key_data_size = string_length / 2;

	return( 1 );

on_error:
	memory_set(
	 mount_handle->key_data,
	 0,
	 64 );

	mount_handle->key_data_size = 0;

	return( -1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_offset(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( mount_handle_system_string_copy_from_64_bit_in_decimal(
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
	mount_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Sets the password
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_password(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_password";
	size_t string_length  = 0;

	if( mount_handle == NULL )
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

	mount_handle->password        = string;
	mount_handle->password_length = string_length;

	return( 1 );
}

/* Sets the path prefix
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_path_prefix(
     mount_handle_t *mount_handle,
     const system_character_t *path_prefix,
     size_t path_prefix_size,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_path_prefix";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_file_system_set_path_prefix(
	     mount_handle->file_system,
	     path_prefix,
	     path_prefix_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set path prefix.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the mount handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int mount_handle_open(
     mount_handle_t *mount_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	system_character_t password[ 64 ];

	libbfio_handle_t *file_io_handle  = NULL;
	libluksde_volume_t *luksde_volume = NULL;
	static char *function             = "mount_handle_open";
	size_t filename_length            = 0;
	size_t password_length            = 0;
	int result                        = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filename_length = system_string_length(
	                   filename );

	if( libbfio_file_range_initialize(
	     &file_io_handle,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to set file range name.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_set(
	     file_io_handle,
	     mount_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to set file range offset.",
		 function );

		goto on_error;
	}
	if( libluksde_volume_initialize(
	     &luksde_volume,
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
	if( mount_handle->key_data_size > 0 )
	{
		if( libluksde_volume_set_key(
		     luksde_volume,
		     mount_handle->key_data,
		     mount_handle->key_data_size,
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
	if( mount_handle->password != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libluksde_volume_set_utf16_password(
		     luksde_volume,
		     (uint16_t *) mount_handle->password,
		     mount_handle->password_length,
		     error ) != 1 )
#else
		if( libluksde_volume_set_utf8_password(
		     luksde_volume,
		     (uint8_t *) mount_handle->password,
		     mount_handle->password_length,
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
	     luksde_volume,
	     file_io_handle,
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
	          luksde_volume,
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
	      && ( mount_handle->unattended_mode == 0 ) )
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
			     luksde_volume,
			     (uint16_t *) password,
			     password_length,
			     error ) != 1 )
#else
			if( libluksde_volume_set_utf8_password(
			     luksde_volume,
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
		          luksde_volume,
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
		result = ( result == 0 );
	}
	if( result != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable unlock volume.",
		 function );

		goto on_error;
	}
	if( mount_file_system_append_volume(
	     mount_handle->file_system,
	     luksde_volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append volume to file system.",
		 function );

		goto on_error;
	}
	mount_handle->file_io_handle = file_io_handle;

	return( 1 );

on_error:
	if( luksde_volume != NULL )
	{
		libluksde_volume_free(
		 &luksde_volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	memory_set(
	 password,
	 0,
	 64 );

	return( -1 );
}

/* Closes the mount handle
 * Returns the 0 if succesful or -1 on error
 */
int mount_handle_close(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	libluksde_volume_t *luksde_volume = NULL;
	static char *function             = "mount_handle_close";
	int number_of_volumes             = 0;
	int volume_index                  = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_file_system_get_number_of_volumes(
	     mount_handle->file_system,
	     &number_of_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of volumes.",
		 function );

		goto on_error;
	}
	for( volume_index = number_of_volumes - 1;
	     volume_index > 0;
	     volume_index-- )
	{
		if( mount_file_system_get_volume_by_index(
		     mount_handle->file_system,
		     volume_index,
		     &luksde_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
/* TODO remove luksde_volume from file system */

		if( libluksde_volume_close(
		     luksde_volume,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
		if( libluksde_volume_free(
		     &luksde_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
	}
	if( libbfio_handle_close(
	     mount_handle->file_io_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to close file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &( mount_handle->file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 0 );

on_error:
	if( luksde_volume != NULL )
	{
		libluksde_volume_free(
		 &luksde_volume,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a file entry for a specific path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int mount_handle_get_file_entry_by_path(
     mount_handle_t *mount_handle,
     const system_character_t *path,
     mount_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libluksde_volume_t *luksde_volume  = NULL;
	const system_character_t *filename = NULL;
	static char *function              = "mount_handle_get_file_entry_by_path";
	size_t filename_length             = 0;
	size_t path_index                  = 0;
	size_t path_length                 = 0;
	int result                         = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	if( path_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid path length value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( path_length >= 2 )
	 && ( path[ path_length - 1 ] == LIBCPATH_SEPARATOR ) )
	{
		path_length--;
	}
	path_index = path_length;

	while( path_index > 0 )
	{
		if( path[ path_index ] == LIBCPATH_SEPARATOR )
		{
			break;
		}
		path_index--;
	}
	/* Ignore the name of the root item
	 */
	if( path_length == 0 )
	{
		filename        = _SYSTEM_STRING( "" );
		filename_length = 0;
	}
	else
	{
		filename        = &( path[ path_index + 1 ] );
		filename_length = path_length - ( path_index + 1 );
	}
	result = mount_file_system_get_volume_by_path(
	          mount_handle->file_system,
	          path,
	          path_length,
	          &luksde_volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( mount_file_entry_initialize(
		     file_entry,
		     mount_handle->file_system,
		     filename,
		     filename_length,
		     luksde_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize file entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	return( -1 );
}

