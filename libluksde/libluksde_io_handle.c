/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2013-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libluksde_codepage.h"
#include "libluksde_definitions.h"
#include "libluksde_encryption.h"
#include "libluksde_io_handle.h"
#include "libluksde_key_slot.h"
#include "libluksde_libbfio.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"
#include "libluksde_libfcache.h"
#include "libluksde_libfdata.h"
#include "libluksde_libuna.h"
#include "libluksde_sector_data.h"
#include "libluksde_unused.h"

#include "luksde_volume.h"

const uint8_t luksde_signature[ 6 ] = { 'L', 'U', 'K', 'S', 0xba, 0xbe };

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_initialize(
     libluksde_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libluksde_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libluksde_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libluksde_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *io_handle )->key_slots_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key slots array.",
		 function );

		goto on_error;
	}
	( *io_handle )->bytes_per_sector = 512;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_free(
     libluksde_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libluksde_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( libluksde_io_handle_clear(
		     *io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			result = -1;
		}
		if( ( *io_handle )->user_password != NULL )
		{
			if( memory_set(
			     ( *io_handle )->user_password,
			     0,
			     ( *io_handle )->user_password_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear user password.",
				 function );

				result = -1;
			}
			memory_free(
			 ( *io_handle )->user_password );

			( *io_handle )->user_password      = NULL;
			( *io_handle )->user_password_size = 0;
		}
		if( libcdata_array_free(
		     &( ( *io_handle )->key_slots_array ),
		     (int(*)(intptr_t **, libcerror_error_t **)) &libluksde_key_slot_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key slots array.",
			 function );

			result = -1;
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_clear(
     libluksde_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libluksde_io_handle_clear";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->encryption_context != NULL )
	{
		if( libluksde_encryption_free(
		     &( io_handle->encryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encryption context.",
			 function );

			result = -1;
		}
	}
	if( memory_set(
	     io_handle->master_key,
	     0,
	     32 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear master key.",
		 function );

		result = -1;
	}
	if( libcdata_array_empty(
	     io_handle->key_slots_array,
	     (int(*)(intptr_t **, libcerror_error_t **)) &libluksde_key_slot_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty key slots array.",
		 function );

		result = -1;
	}
	io_handle->bytes_per_sector = 512;

	return( result );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_read_volume_header(
     libluksde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libluksde_key_slot_t *key_slot = NULL;
	uint8_t *volume_header_data    = NULL;
	uint8_t *volume_key_slot_data  = NULL;
	static char *function          = "libluksde_io_handle_read_volume_header";
	size_t read_size               = 1024;
	ssize_t read_count             = 0;
	int entry_index                = 0;
	int key_slot_index             = 0;
	int separator_index            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *value_string             = NULL;
	size_t value_string_size       = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek volume header offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	volume_header_data = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * read_size );

	if( volume_header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              volume_header_data,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 volume_header_data,
		 read_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (luksde_volume_header_t *) volume_header_data )->signature,
	     luksde_signature,
	     6 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_big_endian(
	 ( (luksde_volume_header_t *) volume_header_data )->version,
	 io_handle->version );

	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_header_t *) volume_header_data )->encrypted_volume_start_sector,
	 io_handle->encrypted_volume_offset );

	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_header_t *) volume_header_data )->master_key_size,
	 io_handle->master_key_size );

        if( memory_copy(
	     io_handle->master_key_validation_hash,
	     ( (luksde_volume_header_t *) volume_header_data )->master_key_validation_hash,
             20 ) == NULL)
        {
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_MEMORY,
                 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
                 "%s: unable to copy master key validation hash.",
                 function );

                goto on_error;
        }
        if( memory_copy(
	     io_handle->master_key_salt,
	     ( (luksde_volume_header_t *) volume_header_data )->master_key_salt,
             32 ) == NULL)
        {
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_MEMORY,
                 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
                 "%s: unable to copy master key salt.",
                 function );

                goto on_error;
        }
	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_header_t *) volume_header_data )->master_key_number_of_iterations,
	 io_handle->master_key_number_of_iterations );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t: %c%c%c%c\\x%02x\\x%02x\n",
		 function,
		 volume_header_data[ 0 ],
		 volume_header_data[ 1 ],
		 volume_header_data[ 2 ],
		 volume_header_data[ 3 ],
		 volume_header_data[ 4 ],
		 volume_header_data[ 5 ] );

		libcnotify_printf(
		 "%s: version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 io_handle->version );

		if( libuna_byte_stream_size_from_utf8(
		     ( (luksde_volume_header_t *) volume_header_data )->encryption_method,
		     32,
		     LIBLUKSDE_CODEPAGE_US_ASCII,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine encryption method string size.",
			 function );

			goto on_error;
		}
		value_string = (char *) memory_allocate(
		                         sizeof( char ) * value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to create encryption method string.",
			 function );

			goto on_error;
		}
		if( libuna_byte_stream_copy_from_utf8(
		     (uint8_t *) value_string,
		     value_string_size,
		     LIBLUKSDE_CODEPAGE_US_ASCII,
		     ( (luksde_volume_header_t *) volume_header_data )->encryption_method,
		     32,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy encryption method string.",
			 function );

			goto on_error;
		}
		value_string[ value_string_size - 1 ] = 0;

		libcnotify_printf(
		 "%s: encryption method\t\t: %s\n",
		 function,
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;

		if( libuna_byte_stream_size_from_utf8(
		     ( (luksde_volume_header_t *) volume_header_data )->encryption_mode,
		     32,
		     LIBLUKSDE_CODEPAGE_US_ASCII,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine encryption mode string size.",
			 function );

			goto on_error;
		}
		value_string = (char *) memory_allocate(
		                         sizeof( char ) * value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to create encryption mode string.",
			 function );

			goto on_error;
		}
		if( libuna_byte_stream_copy_from_utf8(
		     (uint8_t *) value_string,
		     value_string_size,
		     LIBLUKSDE_CODEPAGE_US_ASCII,
		     ( (luksde_volume_header_t *) volume_header_data )->encryption_mode,
		     32,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy encryption mode string.",
			 function );

			goto on_error;
		}
		value_string[ value_string_size - 1 ] = 0;

		libcnotify_printf(
		 "%s: encryption mode\t\t\t: %s\n",
		 function,
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;

		if( libuna_byte_stream_size_from_utf8(
		     ( (luksde_volume_header_t *) volume_header_data )->hashing_method,
		     32,
		     LIBLUKSDE_CODEPAGE_US_ASCII,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine hashing method string size.",
			 function );

			goto on_error;
		}
		value_string = (char *) memory_allocate(
		                         sizeof( char ) * value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to create hashing method string.",
			 function );

			goto on_error;
		}
		if( libuna_byte_stream_copy_from_utf8(
		     (uint8_t *) value_string,
		     value_string_size,
		     LIBLUKSDE_CODEPAGE_US_ASCII,
		     ( (luksde_volume_header_t *) volume_header_data )->hashing_method,
		     32,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy hashing method string.",
			 function );

			goto on_error;
		}
		value_string[ value_string_size - 1 ] = 0;

		libcnotify_printf(
		 "%s: hashing method\t\t\t: %s\n",
		 function,
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;

		libcnotify_printf(
		 "%s: encrypted volume start sector\t: 0x%08" PRIx64 "\n",
		 function,
		 io_handle->encrypted_volume_offset );

		libcnotify_printf(
		 "%s: master key size\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->master_key_size );

		libcnotify_printf(
		 "%s: master key validation hash:\n",
		 function );
		libcnotify_print_data(
		 io_handle->master_key_validation_hash,
		 20,
		 0 );

		libcnotify_printf(
		 "%s: master key salt:\n",
		 function );
		libcnotify_print_data(
		 io_handle->master_key_salt,
		 32,
		 0 );

		libcnotify_printf(
		 "%s: master key number of iterations\t: %" PRIu32 "\n",
		 function,
		 io_handle->master_key_number_of_iterations );

		if( libuna_byte_stream_size_from_utf8(
		     ( (luksde_volume_header_t *) volume_header_data )->volume_identifier,
		     32,
		     LIBLUKSDE_CODEPAGE_US_ASCII,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine volume identifier string size.",
			 function );

			goto on_error;
		}
		value_string = (char *) memory_allocate(
		                         sizeof( char ) * value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to create volume identifier string.",
			 function );

			goto on_error;
		}
		if( libuna_byte_stream_copy_from_utf8(
		     (uint8_t *) value_string,
		     value_string_size,
		     LIBLUKSDE_CODEPAGE_US_ASCII,
		     ( (luksde_volume_header_t *) volume_header_data )->volume_identifier,
		     32,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy volume identifier string.",
			 function );

			goto on_error;
		}
		value_string[ value_string_size - 1 ] = 0;

		libcnotify_printf(
		 "%s: volume identifier\t\t: %s\n",
		 function,
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( io_handle->version != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported version.",
		 function );

		goto on_error;
	}
	if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 'a' )
	 || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 'A' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'e' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'E' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 'S' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 0 ) )
		{
			io_handle->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_AES;
		}
	}
	else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 'b' )
	      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 'B' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'l' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'L' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 'o' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 'O' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 'w' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 'W' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 4 ] == 'f' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 4 ] == 'F' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 5 ] == 'i' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 5 ] == 'I' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 6 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 6 ] == 'S' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 7 ] == 'h' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 7 ] == 'H' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 8 ] == 0 ) )
		{
			io_handle->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_BLOWFISH;
		}
	}
	else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 'c' )
	      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 'C' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'a' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'A' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 'S' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 't' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 'T' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 5 ] == 0 ) )
		{
		 	if( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 4 ] == '5' )
			{
				io_handle->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_CAST5;
			}
		 	else if( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 4 ] == '6' )
			{
				io_handle->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_CAST6;
			}
		}
	}
	else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 's' )
	      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 'S' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'e' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'E' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 'r' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 'R' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 'p' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 'P' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 4 ] == 'e' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 4 ] == 'E' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 5 ] == 'n' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 5 ] == 'N' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 6 ] == 't' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 6 ] == 'T' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 7 ] == 0 ) )
		{
			io_handle->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_SERPENT;
		}
	}
	else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 't' )
	      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 0 ] == 'T' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'w' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 1 ] == 'W' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 'o' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 2 ] == 'O' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 'f' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 3 ] == 'F' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 4 ] == 'i' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 4 ] == 'I' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 5 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 5 ] == 'S' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 6 ] == 'h' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 6 ] == 'H' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_method[ 7 ] == 0 ) )
		{
			io_handle->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_TWOFISH;
		}
	}
	if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 0 ] == 'c' )
	 || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 0 ] == 'C' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 1 ] == 'b' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 1 ] == 'B' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 2 ] == 'c' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 2 ] == 'C' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 3 ] == '-' ) )
		{
			io_handle->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC;

			separator_index = 3;
		}
	}
	else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 0 ] == 'e' )
	      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 0 ] == 'E' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 1 ] == 'c' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 1 ] == 'C' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 2 ] == 'b' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 2 ] == 'B' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 3 ] == 0 ) )
		{
			io_handle->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB;

			separator_index = 3;
		}
	}
	else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 0 ] == 'x' )
	      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 0 ] == 'X' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 1 ] == 't' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 1 ] == 'T' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 2 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 2 ] == 'S' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ 3 ] == '-' ) )
		{
			io_handle->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_XTS;

			separator_index = 3;
		}
	}
	if( ( separator_index > 0 )
	 && ( separator_index < 31 ) )
	{
		if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'b' )
		 || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'B' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'e' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'E' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'n' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'N' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 'b' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 'B' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 5 ] == 'i' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 5 ] == 'I' ) )
			 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 6 ] == 0 ) )
			{
				io_handle->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_BENBI;
			}
		}
		else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'e' )
		      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'E' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 's' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'S' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 's' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'S' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 'i' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 'I' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 5 ] == 'v' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 5 ] == 'V' ) )
			 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 6 ] == ':' ) )
			{
				if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 7 ] == 's' )
				  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 7 ] == 'S' ) )
				 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 8 ] == 'h' )
				  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 8 ] == 'H' ) )
				 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 9 ] == 'a' )
				  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 9 ] == 'A' ) ) )
				{
					if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 10 ] == '1' )
					 && ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 11 ] == 0 ) )
					{
						io_handle->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV;
						io_handle->essiv_hashing_method       = LIBLUKSDE_HASHING_METHOD_SHA1;
					}
					if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 10 ] == '2' )
					 && ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 11 ] == '5' )
					 && ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 12 ] == '6' )
					 && ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 13 ] == 0 ) )
					{
						io_handle->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV;
						io_handle->essiv_hashing_method       = LIBLUKSDE_HASHING_METHOD_SHA256;
					}
				}
/* TODO IV options */
			}
		}
		else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'l' )
		      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'L' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'm' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'M' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'k' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'K' ) )
			 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 0 ) )
			{
				io_handle->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_LMK;
			}
		}
		else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'n' )
		      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'N' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'u' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'U' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'l' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'L' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 'l' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 'L' ) )
			 &&   ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 5 ] == 0 ) )
			{
				io_handle->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_NULL;
			}
		}
		else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'p' )
		      || ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 1 ] == 'P' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'l' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 2 ] == 'L' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'a' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 3 ] == 'A' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 'i' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 4 ] == 'I' ) )
			 && ( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 5 ] == 'n' )
			  ||  ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 5 ] == 'N' ) ) )
			{
				if( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 6 ] == 0 )
				{
					io_handle->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_PLAIN32;
				}
				else if( ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 6 ] == '6' )
				      && ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 7 ] == '4' )
				      && ( ( (luksde_volume_header_t *) volume_header_data )->encryption_mode[ separator_index + 8 ] == 0 ) )
				{
					io_handle->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_PLAIN64;
				}
			}
		}
	}
	if( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 0 ] == 'r' )
	 || ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 0 ] == 'R' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 1 ] == 'i' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 1 ] == 'I' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 2 ] == 'p' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 2 ] == 'P' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 3 ] == 'e' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 3 ] == 'E' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 4 ] == 'm' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 4 ] == 'M' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 5 ] == 'd' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 5 ] == 'D' ) )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 6 ] == '1' )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 7 ] == '6' )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 8 ] == '0' )
		 &&   ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 9 ] == 0 ) )
		{
			io_handle->hashing_method = LIBLUKSDE_HASHING_METHOD_RIPEMD160;
		}
	}
	else if( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 0 ] == 's' )
	      || ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 0 ] == 'S' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 1 ] == 'h' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 1 ] == 'H' ) )
		 && ( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 2 ] == 'a' )
		  ||  ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 2 ] == 'A' ) ) )
		{
			if( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 3 ] == '1' )
			 && ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 4 ] == 0 ) )
			{
				io_handle->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA1;
			}
			else if( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 3 ] == '2' )
			      && ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 4 ] == '2' )
			      && ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 5 ] == '4' )
			      && ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 6 ] == 0 ) )
			{
				io_handle->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA224;
			}
			else if( ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 3 ] == '2' )
			      && ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 4 ] == '5' )
			      && ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 5 ] == '6' )
			      && ( ( (luksde_volume_header_t *) volume_header_data )->hashing_method[ 6 ] == 0 ) )
			{
				io_handle->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA256;
			}
		}
	}
	volume_key_slot_data = volume_header_data + sizeof( luksde_volume_header_t );

	for( key_slot_index = 0;
	     key_slot_index < 8;
	     key_slot_index++ )
	{
		if( libluksde_key_slot_initialize(
		     &key_slot,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key slot.",
			 function );

			goto on_error;
		}
		if( libluksde_key_slot_read_data(
		     key_slot,
		     volume_key_slot_data,
		     sizeof( luksde_volume_key_slot_t ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read key slot data.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     io_handle->key_slots_array,
		     &entry_index,
		     (intptr_t *) key_slot,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to append key slot to key slots array.",
			 function );

			goto on_error;
		}
		key_slot = NULL;

		volume_key_slot_data += sizeof( luksde_volume_key_slot_t );
	}
/* TODO bounds check */
	io_handle->encrypted_volume_offset *= io_handle->bytes_per_sector;

	io_handle->encrypted_volume_size = io_handle->volume_size
	                                 - io_handle->encrypted_volume_offset;

	memory_free(
	 volume_header_data );

	volume_header_data = NULL;

	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
#endif
	if( key_slot != NULL )
	{
		libluksde_key_slot_free(
		 &key_slot,
		 NULL );
	}
	if( volume_header_data != NULL )
	{
		memory_free(
		 volume_header_data );
	}
	libcdata_array_empty(
	 io_handle->key_slots_array,
	 (int(*)(intptr_t **, libcerror_error_t **)) &libluksde_key_slot_free,
	 NULL );

	return( -1 );
}

/* Reads a sector
 * Callback function for the volume vector
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_read_sector(
     libluksde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index LIBLUKSDE_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size LIBLUKSDE_ATTRIBUTE_UNUSED,
     uint32_t element_data_flags LIBLUKSDE_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBLUKSDE_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libluksde_sector_data_t *sector_data = NULL;
	static char *function                = "libluksde_io_handle_read_sector";

	LIBLUKSDE_UNREFERENCED_PARAMETER( element_data_file_index );
	LIBLUKSDE_UNREFERENCED_PARAMETER( element_data_size );
	LIBLUKSDE_UNREFERENCED_PARAMETER( element_data_flags );
	LIBLUKSDE_UNREFERENCED_PARAMETER( read_flags );

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
/* TODO handle virtual sectors, what about different sector sizes? */
	if( libluksde_sector_data_initialize(
	     &sector_data,
	     (size_t) io_handle->bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( libluksde_sector_data_read(
	     sector_data,
	     io_handle,
	     file_io_handle,
	     element_data_offset,
	     io_handle->encryption_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sector data.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) sector_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libluksde_sector_data_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sector data as element value.",
		 function );

		return( -1 );
	}
	return( 1 );

on_error:
	if( sector_data != NULL )
	{
		libluksde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( -1 );
}

