/*
 * The LUKS volume header functions
 *
 * Copyright (C) 2013-2020, Joachim Metz <joachim.metz@gmail.com>
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

#include "libluksde_debug.h"
#include "libluksde_definitions.h"
#include "libluksde_io_handle.h"
#include "libluksde_key_slot.h"
#include "libluksde_libbfio.h"
#include "libluksde_libcdata.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"
#include "libluksde_libfguid.h"
#include "libluksde_libuna.h"
#include "libluksde_volume_header.h"

#include "luksde_volume.h"

/* Creates a volume header
 * Make sure the value volume_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_header_initialize(
     libluksde_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libluksde_volume_header_initialize";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume header value already set.",
		 function );

		return( -1 );
	}
	*volume_header = memory_allocate_structure(
	                  libluksde_volume_header_t );

	if( *volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_header,
	     0,
	     sizeof( libluksde_volume_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume header.",
		 function );

		memory_free(
		 *volume_header );

		*volume_header = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *volume_header )->key_slots_array ),
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
	return( 1 );

on_error:
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( -1 );
}

/* Frees a volume header
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_header_free(
     libluksde_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libluksde_volume_header_free";
	int result            = 1;

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		if( libcdata_array_free(
		     &( ( *volume_header )->key_slots_array ),
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
		 *volume_header );

		*volume_header = NULL;
	}
	return( result );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_header_read_data(
     libluksde_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfguid_identifier_t *guid    = NULL;
	libluksde_key_slot_t *key_slot = NULL;
	static char *function          = "libluksde_volume_header_read_data";
	size_t data_offset             = 0;
	int entry_index                = 0;
	int key_slot_index             = 0;
	int separator_index            = 0;

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( luksde_volume_header_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( luksde_volume_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (luksde_volume_header_t *) data )->signature,
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
	 ( (luksde_volume_header_t *) data )->version,
	 volume_header->version );

	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_header_t *) data )->encrypted_volume_start_sector,
	 volume_header->encrypted_volume_offset );

	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_header_t *) data )->master_key_size,
	 volume_header->master_key_size );

        if( memory_copy(
	     volume_header->master_key_validation_hash,
	     ( (luksde_volume_header_t *) data )->master_key_validation_hash,
             20 ) == NULL )
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
	     volume_header->master_key_salt,
	     ( (luksde_volume_header_t *) data )->master_key_salt,
             32 ) == NULL )
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
	 ( (luksde_volume_header_t *) data )->master_key_number_of_iterations,
	 volume_header->master_key_number_of_iterations );

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
	if( libfguid_identifier_copy_from_utf8_string(
	     guid,
	     ( (luksde_volume_header_t *) data )->volume_identifier,
	     40,
	     LIBFGUID_STRING_FORMAT_FLAG_USE_MIXED_CASE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy GUID from string.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_to_byte_stream(
	     guid,
	     volume_header->volume_identifier,
	     16,
	     LIBFGUID_ENDIAN_BIG,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy GUID to byte stream.",
		 function );

		goto on_error;
	}
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\\x%02x\\x%02x\n",
		 function,
		 data[ 0 ],
		 data[ 1 ],
		 data[ 2 ],
		 data[ 3 ],
		 data[ 4 ],
		 data[ 5 ] );

		libcnotify_printf(
		 "%s: version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 volume_header->version );

		if( libluksde_debug_print_string_value(
		     function,
		     "encryption method\t\t\t",
		     ( (luksde_volume_header_t *) data )->encryption_method,
		     32,
		     LIBUNA_CODEPAGE_US_ASCII,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print string value.",
			 function );

			goto on_error;
		}
		if( libluksde_debug_print_string_value(
		     function,
		     "encryption mode\t\t\t",
		     ( (luksde_volume_header_t *) data )->encryption_mode,
		     32,
		     LIBUNA_CODEPAGE_US_ASCII,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print string value.",
			 function );

			goto on_error;
		}
		if( libluksde_debug_print_string_value(
		     function,
		     "hashing method\t\t\t",
		     ( (luksde_volume_header_t *) data )->hashing_method,
		     32,
		     LIBUNA_CODEPAGE_US_ASCII,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print string value.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: encrypted volume start sector\t: 0x%08" PRIx64 "\n",
		 function,
		 volume_header->encrypted_volume_offset );

		libcnotify_printf(
		 "%s: master key size\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->master_key_size );

		libcnotify_printf(
		 "%s: master key validation hash:\n",
		 function );
		libcnotify_print_data(
		 volume_header->master_key_validation_hash,
		 20,
		 0 );

		libcnotify_printf(
		 "%s: master key salt:\n",
		 function );
		libcnotify_print_data(
		 volume_header->master_key_salt,
		 32,
		 0 );

		libcnotify_printf(
		 "%s: master key number of iterations\t: %" PRIu32 "\n",
		 function,
		 volume_header->master_key_number_of_iterations );

		if( libluksde_debug_print_string_value(
		     function,
		     "volume identifier\t\t\t",
		     ( (luksde_volume_header_t *) data )->volume_identifier,
		     40,
		     LIBUNA_CODEPAGE_US_ASCII,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print string value.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( volume_header->version != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported version.",
		 function );

		goto on_error;
	}
/* TODO move encryption method detection into separate function */
	if( ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 'a' )
	 || ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 'A' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'e' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'E' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'S' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_AES;
		}
		else if( ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'r' )
		      ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'R' ) )
		     && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'c' )
		      ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'C' ) )
		     &&   ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == '4' )
		     &&   ( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_ARC4;
		}
	}
	else if( ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 'b' )
	      || ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 'B' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'l' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'L' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'o' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'O' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 'w' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 'W' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == 'f' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == 'F' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 5 ] == 'i' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 5 ] == 'I' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 6 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 6 ] == 'S' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 7 ] == 'h' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 7 ] == 'H' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->encryption_method[ 8 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_BLOWFISH;
		}
	}
	else if( ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 'c' )
	      || ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 'C' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'a' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'A' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'S' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 't' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 'T' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->encryption_method[ 5 ] == 0 ) )
		{
		 	if( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == '5' )
			{
				volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_CAST5;
			}
		 	else if( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == '6' )
			{
				volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_CAST6;
			}
		}
	}
	else if( ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 's' )
	      || ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 'S' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'e' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'E' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'r' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'R' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 'p' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 'P' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == 'e' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == 'E' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 5 ] == 'n' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 5 ] == 'N' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 6 ] == 't' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 6 ] == 'T' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->encryption_method[ 7 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_SERPENT;
		}
	}
	else if( ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 't' )
	      || ( ( (luksde_volume_header_t *) data )->encryption_method[ 0 ] == 'T' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'w' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 1 ] == 'W' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'o' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 2 ] == 'O' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 'f' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 3 ] == 'F' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == 'i' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 4 ] == 'I' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 5 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 5 ] == 'S' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_method[ 6 ] == 'h' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_method[ 6 ] == 'H' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->encryption_method[ 7 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_TWOFISH;
		}
	}
	if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 0 ] == 'c' )
	 || ( ( (luksde_volume_header_t *) data )->encryption_mode[ 0 ] == 'C' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 1 ] == 'b' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ 1 ] == 'B' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 2 ] == 'c' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ 2 ] == 'C' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->encryption_mode[ 3 ] == '-' ) )
		{
			volume_header->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC;

			separator_index = 3;
		}
	}
	else if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 0 ] == 'e' )
	      || ( ( (luksde_volume_header_t *) data )->encryption_mode[ 0 ] == 'E' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 1 ] == 'c' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ 1 ] == 'C' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 2 ] == 'b' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ 2 ] == 'B' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->encryption_mode[ 3 ] == 0 ) )
		{
			volume_header->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB;

			separator_index = 3;
		}
	}
	else if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 0 ] == 'x' )
	      || ( ( (luksde_volume_header_t *) data )->encryption_mode[ 0 ] == 'X' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 1 ] == 't' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ 1 ] == 'T' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ 2 ] == 's' )
		  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ 2 ] == 'S' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->encryption_mode[ 3 ] == '-' ) )
		{
			volume_header->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_XTS;

			separator_index = 3;
		}
	}
	if( ( separator_index > 0 )
	 && ( separator_index < 31 ) )
	{
		volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_NONE;

		if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'b' )
		 || ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'B' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'e' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'E' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'n' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'N' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 'b' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 'B' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 5 ] == 'i' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 5 ] == 'I' ) )
			 &&   ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 6 ] == 0 ) )
			{
				volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_BENBI;
			}
		}
		else if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'e' )
		      || ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'E' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 's' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'S' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 's' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'S' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 'i' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 'I' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 5 ] == 'v' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 5 ] == 'V' ) )
			 &&   ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 6 ] == ':' ) )
			{
				if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 7 ] == 's' )
				  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 7 ] == 'S' ) )
				 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 8 ] == 'h' )
				  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 8 ] == 'H' ) )
				 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 9 ] == 'a' )
				  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 9 ] == 'A' ) ) )
				{
					if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 10 ] == '1' )
					 && ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 11 ] == 0 ) )
					{
						volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV;
						volume_header->essiv_hashing_method       = LIBLUKSDE_HASHING_METHOD_SHA1;
					}
					else if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 10 ] == '2' )
					      && ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 11 ] == '5' )
					      && ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 12 ] == '6' )
					      && ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 13 ] == 0 ) )
					{
						volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV;
						volume_header->essiv_hashing_method       = LIBLUKSDE_HASHING_METHOD_SHA256;
					}
				}
/* TODO IV options */
			}
		}
		else if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'l' )
		      || ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'L' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'm' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'M' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'k' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'K' ) )
			 &&   ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 0 ) )
			{
				volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_LMK;
			}
		}
		else if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'n' )
		      || ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'N' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'u' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'U' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'l' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'L' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 'l' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 'L' ) )
			 &&   ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 5 ] == 0 ) )
			{
				volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_NULL;
			}
		}
		else if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'p' )
		      || ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 1 ] == 'P' ) )
		{
			if( ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'l' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 2 ] == 'L' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'a' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 3 ] == 'A' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 'i' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 4 ] == 'I' ) )
			 && ( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 5 ] == 'n' )
			  ||  ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 5 ] == 'N' ) ) )
			{
				if( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 6 ] == 0 )
				{
					volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_PLAIN32;
				}
				else if( ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 6 ] == '6' )
				      && ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 7 ] == '4' )
				      && ( ( (luksde_volume_header_t *) data )->encryption_mode[ separator_index + 8 ] == 0 ) )
				{
					volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_PLAIN64;
				}
			}
		}
	}
	if( ( ( (luksde_volume_header_t *) data )->hashing_method[ 0 ] == 'r' )
	 || ( ( (luksde_volume_header_t *) data )->hashing_method[ 0 ] == 'R' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->hashing_method[ 1 ] == 'i' )
		  ||  ( ( (luksde_volume_header_t *) data )->hashing_method[ 1 ] == 'I' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->hashing_method[ 2 ] == 'p' )
		  ||  ( ( (luksde_volume_header_t *) data )->hashing_method[ 2 ] == 'P' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->hashing_method[ 3 ] == 'e' )
		  ||  ( ( (luksde_volume_header_t *) data )->hashing_method[ 3 ] == 'E' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->hashing_method[ 4 ] == 'm' )
		  ||  ( ( (luksde_volume_header_t *) data )->hashing_method[ 4 ] == 'M' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->hashing_method[ 5 ] == 'd' )
		  ||  ( ( (luksde_volume_header_t *) data )->hashing_method[ 5 ] == 'D' ) )
		 &&   ( ( (luksde_volume_header_t *) data )->hashing_method[ 6 ] == '1' )
		 &&   ( ( (luksde_volume_header_t *) data )->hashing_method[ 7 ] == '6' )
		 &&   ( ( (luksde_volume_header_t *) data )->hashing_method[ 8 ] == '0' )
		 &&   ( ( (luksde_volume_header_t *) data )->hashing_method[ 9 ] == 0 ) )
		{
			volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_RIPEMD160;
		}
	}
	else if( ( ( (luksde_volume_header_t *) data )->hashing_method[ 0 ] == 's' )
	      || ( ( (luksde_volume_header_t *) data )->hashing_method[ 0 ] == 'S' ) )
	{
		if( ( ( ( (luksde_volume_header_t *) data )->hashing_method[ 1 ] == 'h' )
		  ||  ( ( (luksde_volume_header_t *) data )->hashing_method[ 1 ] == 'H' ) )
		 && ( ( ( (luksde_volume_header_t *) data )->hashing_method[ 2 ] == 'a' )
		  ||  ( ( (luksde_volume_header_t *) data )->hashing_method[ 2 ] == 'A' ) ) )
		{
			if( ( ( (luksde_volume_header_t *) data )->hashing_method[ 3 ] == '1' )
			 && ( ( (luksde_volume_header_t *) data )->hashing_method[ 4 ] == 0 ) )
			{
				volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA1;
			}
			else if( ( ( (luksde_volume_header_t *) data )->hashing_method[ 3 ] == '2' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 4 ] == '2' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 5 ] == '4' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 6 ] == 0 ) )
			{
				volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA224;
			}
			else if( ( ( (luksde_volume_header_t *) data )->hashing_method[ 3 ] == '2' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 4 ] == '5' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 5 ] == '6' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 6 ] == 0 ) )
			{
				volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA256;
			}
			else if( ( ( (luksde_volume_header_t *) data )->hashing_method[ 3 ] == '5' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 4 ] == '1' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 5 ] == '2' )
			      && ( ( (luksde_volume_header_t *) data )->hashing_method[ 6 ] == 0 ) )
			{
				volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA512;
			}
		}
	}
	data_offset = sizeof( luksde_volume_header_t );

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
		     &( data[ data_offset ] ),
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
		     volume_header->key_slots_array,
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

		data_offset += sizeof( luksde_volume_key_slot_t );
	}
	return( 1 );

on_error:
	if( key_slot != NULL )
	{
		libluksde_key_slot_free(
		 &key_slot,
		 NULL );
	}
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	libcdata_array_empty(
	 volume_header->key_slots_array,
	 (int(*)(intptr_t **, libcerror_error_t **)) &libluksde_key_slot_free,
	 NULL );

	return( -1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_header_read_file_io_handle(
     libluksde_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t volume_header_data[ 1024 ];

	static char *function = "libluksde_volume_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
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
		 "%s: unable to seek volume header offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              volume_header_data,
	              1024,
	              error );

	if( read_count != (ssize_t) 1024 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		return( -1 );
	}
	if( libluksde_volume_header_read_data(
	     volume_header,
	     volume_header_data,
	     1024,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the volume identifier
 * The identifier is an UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_header_get_volume_identifier(
     libluksde_volume_header_t *volume_header,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libluksde_volume_header_get_volume_identifier";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( uuid_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UUID data.",
		 function );

		return( -1 );
	}
	if( uuid_data_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UUID data value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     uuid_data,
	     volume_header->volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set volume identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

