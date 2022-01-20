/*
 * The LUKS volume header functions
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

#include "luksde_keyslot.h"
#include "luksde_volume.h"

const uint8_t luksde_backup_volume_header_signature[ 6 ] = { 'S', 'K', 'U', 'L', 0xba, 0xbe };

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

/* Parses the encryption method
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_parse_encryption_method(
     libluksde_volume_header_t *volume_header,
     const uint8_t *encryption_method,
     libcerror_error_t **error )
{
	static char *function = "libluksde_volume_parse_encryption_method";

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
	if( encryption_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid encryption method.",
		 function );

		return( -1 );
	}
	if( ( encryption_method[ 0 ] == 'a' )
	 || ( encryption_method[ 0 ] == 'A' ) )
	{
		if( ( ( encryption_method[ 1 ] == 'e' )
		  ||  ( encryption_method[ 1 ] == 'E' ) )
		 && ( ( encryption_method[ 2 ] == 's' )
		  ||  ( encryption_method[ 2 ] == 'S' ) )
		 &&   ( encryption_method[ 3 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_AES;
		}
		else if( ( ( encryption_method[ 1 ] == 'r' )
		       ||  ( encryption_method[ 1 ] == 'R' ) )
		      && ( ( encryption_method[ 2 ] == 'c' )
		       ||  ( encryption_method[ 2 ] == 'C' ) )
		      &&   ( encryption_method[ 3 ] == '4' )
		      &&   ( encryption_method[ 4 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_ARC4;
		}
	}
	else if( ( encryption_method[ 0 ] == 'b' )
	      || ( encryption_method[ 0 ] == 'B' ) )
	{
		if( ( ( encryption_method[ 1 ] == 'l' )
		  ||  ( encryption_method[ 1 ] == 'L' ) )
		 && ( ( encryption_method[ 2 ] == 'o' )
		  ||  ( encryption_method[ 2 ] == 'O' ) )
		 && ( ( encryption_method[ 3 ] == 'w' )
		  ||  ( encryption_method[ 3 ] == 'W' ) )
		 && ( ( encryption_method[ 4 ] == 'f' )
		  ||  ( encryption_method[ 4 ] == 'F' ) )
		 && ( ( encryption_method[ 5 ] == 'i' )
		  ||  ( encryption_method[ 5 ] == 'I' ) )
		 && ( ( encryption_method[ 6 ] == 's' )
		  ||  ( encryption_method[ 6 ] == 'S' ) )
		 && ( ( encryption_method[ 7 ] == 'h' )
		  ||  ( encryption_method[ 7 ] == 'H' ) )
		 &&   ( encryption_method[ 8 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_BLOWFISH;
		}
	}
	else if( ( encryption_method[ 0 ] == 'c' )
	      || ( encryption_method[ 0 ] == 'C' ) )
	{
		if( ( ( encryption_method[ 1 ] == 'a' )
		  ||  ( encryption_method[ 1 ] == 'A' ) )
		 && ( ( encryption_method[ 2 ] == 's' )
		  ||  ( encryption_method[ 2 ] == 'S' ) )
		 && ( ( encryption_method[ 3 ] == 't' )
		  ||  ( encryption_method[ 3 ] == 'T' ) )
		 &&   ( encryption_method[ 5 ] == 0 ) )
		{
			if( encryption_method[ 4 ] == '5' )
			{
				volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_CAST5;
			}
			else if( encryption_method[ 4 ] == '6' )
			{
				volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_CAST6;
			}
		}
	}
	else if( ( encryption_method[ 0 ] == 's' )
	      || ( encryption_method[ 0 ] == 'S' ) )
	{
		if( ( ( encryption_method[ 1 ] == 'e' )
		  ||  ( encryption_method[ 1 ] == 'E' ) )
		 && ( ( encryption_method[ 2 ] == 'r' )
		  ||  ( encryption_method[ 2 ] == 'R' ) )
		 && ( ( encryption_method[ 3 ] == 'p' )
		  ||  ( encryption_method[ 3 ] == 'P' ) )
		 && ( ( encryption_method[ 4 ] == 'e' )
		  ||  ( encryption_method[ 4 ] == 'E' ) )
		 && ( ( encryption_method[ 5 ] == 'n' )
		  ||  ( encryption_method[ 5 ] == 'N' ) )
		 && ( ( encryption_method[ 6 ] == 't' )
		  ||  ( encryption_method[ 6 ] == 'T' ) )
		 &&   ( encryption_method[ 7 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_SERPENT;
		}
	}
	else if( ( encryption_method[ 0 ] == 't' )
	      || ( encryption_method[ 0 ] == 'T' ) )
	{
		if( ( ( encryption_method[ 1 ] == 'w' )
		  ||  ( encryption_method[ 1 ] == 'W' ) )
		 && ( ( encryption_method[ 2 ] == 'o' )
		  ||  ( encryption_method[ 2 ] == 'O' ) )
		 && ( ( encryption_method[ 3 ] == 'f' )
		  ||  ( encryption_method[ 3 ] == 'F' ) )
		 && ( ( encryption_method[ 4 ] == 'i' )
		  ||  ( encryption_method[ 4 ] == 'I' ) )
		 && ( ( encryption_method[ 5 ] == 's' )
		  ||  ( encryption_method[ 5 ] == 'S' ) )
		 && ( ( encryption_method[ 6 ] == 'h' )
		  ||  ( encryption_method[ 6 ] == 'H' ) )
		 &&   ( encryption_method[ 7 ] == 0 ) )
		{
			volume_header->encryption_method = LIBLUKSDE_ENCRYPTION_METHOD_TWOFISH;
		}
	}
	return( 1 );
}

/* Parses the encryption mode
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_parse_encryption_mode(
     libluksde_volume_header_t *volume_header,
     const uint8_t *encryption_mode,
     libcerror_error_t **error )
{
	static char *function = "libluksde_volume_parse_encryption_mode";
	int separator_index   = 0;

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
	if( encryption_mode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid encryption mode.",
		 function );

		return( -1 );
	}
	if( ( encryption_mode[ 0 ] == 'c' )
	 || ( encryption_mode[ 0 ] == 'C' ) )
	{
		if( ( ( encryption_mode[ 1 ] == 'b' )
		  ||  ( encryption_mode[ 1 ] == 'B' ) )
		 && ( ( encryption_mode[ 2 ] == 'c' )
		  ||  ( encryption_mode[ 2 ] == 'C' ) )
		 &&   ( encryption_mode[ 3 ] == '-' ) )
		{
			volume_header->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC;

			separator_index = 3;
		}
	}
	else if( ( encryption_mode[ 0 ] == 'e' )
	      || ( encryption_mode[ 0 ] == 'E' ) )
	{
		if( ( ( encryption_mode[ 1 ] == 'c' )
		  ||  ( encryption_mode[ 1 ] == 'C' ) )
		 && ( ( encryption_mode[ 2 ] == 'b' )
		  ||  ( encryption_mode[ 2 ] == 'B' ) )
		 &&   ( encryption_mode[ 3 ] == 0 ) )
		{
			volume_header->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB;

			separator_index = 3;
		}
	}
	else if( ( encryption_mode[ 0 ] == 'x' )
	      || ( encryption_mode[ 0 ] == 'X' ) )
	{
		if( ( ( encryption_mode[ 1 ] == 't' )
		  ||  ( encryption_mode[ 1 ] == 'T' ) )
		 && ( ( encryption_mode[ 2 ] == 's' )
		  ||  ( encryption_mode[ 2 ] == 'S' ) )
		 &&   ( encryption_mode[ 3 ] == '-' ) )
		{
			volume_header->encryption_chaining_mode = LIBLUKSDE_ENCRYPTION_CHAINING_MODE_XTS;

			separator_index = 3;
		}
	}
	if( ( separator_index > 0 )
	 && ( separator_index < 31 ) )
	{
		volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_NONE;

		if( ( encryption_mode[ separator_index + 1 ] == 'b' )
		 || ( encryption_mode[ separator_index + 1 ] == 'B' ) )
		{
			if( ( ( encryption_mode[ separator_index + 2 ] == 'e' )
			  ||  ( encryption_mode[ separator_index + 2 ] == 'E' ) )
			 && ( ( encryption_mode[ separator_index + 3 ] == 'n' )
			  ||  ( encryption_mode[ separator_index + 3 ] == 'N' ) )
			 && ( ( encryption_mode[ separator_index + 4 ] == 'b' )
			  ||  ( encryption_mode[ separator_index + 4 ] == 'B' ) )
			 && ( ( encryption_mode[ separator_index + 5 ] == 'i' )
			  ||  ( encryption_mode[ separator_index + 5 ] == 'I' ) )
			 &&   ( encryption_mode[ separator_index + 6 ] == 0 ) )
			{
				volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_BENBI;
			}
		}
		else if( ( encryption_mode[ separator_index + 1 ] == 'e' )
		      || ( encryption_mode[ separator_index + 1 ] == 'E' ) )
		{
			if( ( ( encryption_mode[ separator_index + 2 ] == 's' )
			  ||  ( encryption_mode[ separator_index + 2 ] == 'S' ) )
			 && ( ( encryption_mode[ separator_index + 3 ] == 's' )
			  ||  ( encryption_mode[ separator_index + 3 ] == 'S' ) )
			 && ( ( encryption_mode[ separator_index + 4 ] == 'i' )
			  ||  ( encryption_mode[ separator_index + 4 ] == 'I' ) )
			 && ( ( encryption_mode[ separator_index + 5 ] == 'v' )
			  ||  ( encryption_mode[ separator_index + 5 ] == 'V' ) )
			 &&   ( encryption_mode[ separator_index + 6 ] == ':' ) )
			{
				if( ( ( encryption_mode[ separator_index + 7 ] == 's' )
				  ||  ( encryption_mode[ separator_index + 7 ] == 'S' ) )
				 && ( ( encryption_mode[ separator_index + 8 ] == 'h' )
				  ||  ( encryption_mode[ separator_index + 8 ] == 'H' ) )
				 && ( ( encryption_mode[ separator_index + 9 ] == 'a' )
				  ||  ( encryption_mode[ separator_index + 9 ] == 'A' ) ) )
				{
					if( ( encryption_mode[ separator_index + 10 ] == '1' )
					 && ( encryption_mode[ separator_index + 11 ] == 0 ) )
					{
						volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV;
						volume_header->essiv_hashing_method       = LIBLUKSDE_HASHING_METHOD_SHA1;
					}
					else if( ( encryption_mode[ separator_index + 10 ] == '2' )
					      && ( encryption_mode[ separator_index + 11 ] == '5' )
					      && ( encryption_mode[ separator_index + 12 ] == '6' )
					      && ( encryption_mode[ separator_index + 13 ] == 0 ) )
					{
						volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV;
						volume_header->essiv_hashing_method       = LIBLUKSDE_HASHING_METHOD_SHA256;
					}
				}
/* TODO IV options */
			}
		}
		else if( ( encryption_mode[ separator_index + 1 ] == 'l' )
		      || ( encryption_mode[ separator_index + 1 ] == 'L' ) )
		{
			if( ( ( encryption_mode[ separator_index + 2 ] == 'm' )
			  ||  ( encryption_mode[ separator_index + 2 ] == 'M' ) )
			 && ( ( encryption_mode[ separator_index + 3 ] == 'k' )
			  ||  ( encryption_mode[ separator_index + 3 ] == 'K' ) )
			 &&   ( encryption_mode[ separator_index + 4 ] == 0 ) )
			{
				volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_LMK;
			}
		}
		else if( ( encryption_mode[ separator_index + 1 ] == 'n' )
		      || ( encryption_mode[ separator_index + 1 ] == 'N' ) )
		{
			if( ( ( encryption_mode[ separator_index + 2 ] == 'u' )
			  ||  ( encryption_mode[ separator_index + 2 ] == 'U' ) )
			 && ( ( encryption_mode[ separator_index + 3 ] == 'l' )
			  ||  ( encryption_mode[ separator_index + 3 ] == 'L' ) )
			 && ( ( encryption_mode[ separator_index + 4 ] == 'l' )
			  ||  ( encryption_mode[ separator_index + 4 ] == 'L' ) )
			 &&   ( encryption_mode[ separator_index + 5 ] == 0 ) )
			{
				volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_NULL;
			}
		}
		else if( ( encryption_mode[ separator_index + 1 ] == 'p' )
		      || ( encryption_mode[ separator_index + 1 ] == 'P' ) )
		{
			if( ( ( encryption_mode[ separator_index + 2 ] == 'l' )
			  ||  ( encryption_mode[ separator_index + 2 ] == 'L' ) )
			 && ( ( encryption_mode[ separator_index + 3 ] == 'a' )
			  ||  ( encryption_mode[ separator_index + 3 ] == 'A' ) )
			 && ( ( encryption_mode[ separator_index + 4 ] == 'i' )
			  ||  ( encryption_mode[ separator_index + 4 ] == 'I' ) )
			 && ( ( encryption_mode[ separator_index + 5 ] == 'n' )
			  ||  ( encryption_mode[ separator_index + 5 ] == 'N' ) ) )
			{
				if( encryption_mode[ separator_index + 6 ] == 0 )
				{
					volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_PLAIN32;
				}
				else if( ( encryption_mode[ separator_index + 6 ] == '6' )
				      && ( encryption_mode[ separator_index + 7 ] == '4' )
				      && ( encryption_mode[ separator_index + 8 ] == 0 ) )
				{
					volume_header->initialization_vector_mode = LIBLUKSDE_INITIALIZATION_VECTOR_MODE_PLAIN64;
				}
			}
		}
	}
	return( 1 );
}

/* Parses the hashing method
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_parse_hashing_method(
     libluksde_volume_header_t *volume_header,
     const uint8_t *hashing_method,
     libcerror_error_t **error )
{
	static char *function = "libluksde_volume_parse_hashing_method";

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
	if( hashing_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid hashing method.",
		 function );

		return( -1 );
	}
	if( ( hashing_method[ 0 ] == 'r' )
	 || ( hashing_method[ 0 ] == 'R' ) )
	{
		if( ( ( hashing_method[ 1 ] == 'i' )
		  ||  ( hashing_method[ 1 ] == 'I' ) )
		 && ( ( hashing_method[ 2 ] == 'p' )
		  ||  ( hashing_method[ 2 ] == 'P' ) )
		 && ( ( hashing_method[ 3 ] == 'e' )
		  ||  ( hashing_method[ 3 ] == 'E' ) )
		 && ( ( hashing_method[ 4 ] == 'm' )
		  ||  ( hashing_method[ 4 ] == 'M' ) )
		 && ( ( hashing_method[ 5 ] == 'd' )
		  ||  ( hashing_method[ 5 ] == 'D' ) )
		 &&   ( hashing_method[ 6 ] == '1' )
		 &&   ( hashing_method[ 7 ] == '6' )
		 &&   ( hashing_method[ 8 ] == '0' )
		 &&   ( hashing_method[ 9 ] == 0 ) )
		{
			volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_RIPEMD160;
		}
	}
	else if( ( hashing_method[ 0 ] == 's' )
	      || ( hashing_method[ 0 ] == 'S' ) )
	{
		if( ( ( hashing_method[ 1 ] == 'h' )
		  ||  ( hashing_method[ 1 ] == 'H' ) )
		 && ( ( hashing_method[ 2 ] == 'a' )
		  ||  ( hashing_method[ 2 ] == 'A' ) ) )
		{
			if( ( hashing_method[ 3 ] == '1' )
			 && ( hashing_method[ 4 ] == 0 ) )
			{
				volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA1;
			}
			else if( ( hashing_method[ 3 ] == '2' )
			      && ( hashing_method[ 4 ] == '2' )
			      && ( hashing_method[ 5 ] == '4' )
			      && ( hashing_method[ 6 ] == 0 ) )
			{
				volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA224;
			}
			else if( ( hashing_method[ 3 ] == '2' )
			      && ( hashing_method[ 4 ] == '5' )
			      && ( hashing_method[ 5 ] == '6' )
			      && ( hashing_method[ 6 ] == 0 ) )
			{
				volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA256;
			}
			else if( ( hashing_method[ 3 ] == '5' )
			      && ( hashing_method[ 4 ] == '1' )
			      && ( hashing_method[ 5 ] == '2' )
			      && ( hashing_method[ 6 ] == 0 ) )
			{
				volume_header->hashing_method = LIBLUKSDE_HASHING_METHOD_SHA512;
			}
		}
	}
	return( 1 );
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
	int result                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit           = 0;
#endif

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
	if( ( data_size < sizeof( luksde_volume_header_v1_t ) )
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
		 sizeof( luksde_volume_header_v1_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (luksde_volume_header_v1_t *) data )->signature,
	     luksde_signature,
	     6 ) == 0 )
	{
	}
	else if( memory_compare(
	          ( (luksde_volume_header_v1_t *) data )->signature,
	          luksde_backup_volume_header_signature,
	          6 ) == 0 )
	{
	}
	else
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
	 ( (luksde_volume_header_v1_t *) data )->format_version,
	 volume_header->format_version );

	if( ( volume_header->format_version != 1 )
	 && ( volume_header->format_version != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		goto on_error;
	}
	if( volume_header->format_version == 2 )
	{
		if( data_size < sizeof( luksde_volume_header_v2_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
	}
	if( volume_header->format_version == 1 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (luksde_volume_header_v1_t *) data )->encrypted_volume_start_sector,
		 volume_header->encrypted_volume_start_sector );

		byte_stream_copy_to_uint32_big_endian(
		 ( (luksde_volume_header_v1_t *) data )->master_key_size,
		 volume_header->master_key_size );

		if( memory_copy(
		     volume_header->master_key_validation_hash,
		     ( (luksde_volume_header_v1_t *) data )->master_key_validation_hash,
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
		     ( (luksde_volume_header_v1_t *) data )->master_key_salt,
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
		 ( (luksde_volume_header_v1_t *) data )->master_key_number_of_iterations,
		 volume_header->master_key_number_of_iterations );
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
	if( volume_header->format_version == 1 )
	{
		result = libfguid_identifier_copy_from_utf8_string(
		          guid,
		          ( (luksde_volume_header_v1_t *) data )->volume_identifier,
		          40,
		          LIBFGUID_STRING_FORMAT_FLAG_USE_MIXED_CASE,
		          error );
	}
	else if( volume_header->format_version == 2 )
	{
		result = libfguid_identifier_copy_from_utf8_string(
		          guid,
		          ( (luksde_volume_header_v2_t *) data )->volume_identifier,
		          40,
		          LIBFGUID_STRING_FORMAT_FLAG_USE_MIXED_CASE,
		          error );
	}
	if( result != 1 )
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
		 "%s: format version\t\t\t: %" PRIu16 "\n",
		 function,
		 volume_header->format_version );

		if( volume_header->format_version == 1 )
		{
			if( libluksde_debug_print_string_value(
			     function,
			     "encryption method\t\t\t",
			     ( (luksde_volume_header_v1_t *) data )->encryption_method,
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
			     ( (luksde_volume_header_v1_t *) data )->encryption_mode,
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
			     ( (luksde_volume_header_v1_t *) data )->hashing_method,
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
			 "%s: encrypted volume start sector\t: %" PRIu64 "\n",
			 function,
			 volume_header->encrypted_volume_start_sector );

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
			     ( (luksde_volume_header_v1_t *) data )->volume_identifier,
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
		}
		else if( volume_header->format_version == 2 )
		{
			byte_stream_copy_to_uint64_big_endian(
			 ( (luksde_volume_header_v2_t *) data )->metadata_area_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: metadata area size\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (luksde_volume_header_v2_t *) data )->sequence_identifier,
			 value_64bit );
			libcnotify_printf(
			 "%s: sequence identifier\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			if( libluksde_debug_print_string_value(
			     function,
			     "label\t\t\t\t",
			     ( (luksde_volume_header_v2_t *) data )->label,
			     48,
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
			     "checksum method\t\t\t",
			     ( (luksde_volume_header_v2_t *) data )->checksum_method,
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
			 "%s: salt:\n",
			 function );
			libcnotify_print_data(
			 ( (luksde_volume_header_v2_t *) data )->salt,
			 64,
			 0 );

			if( libluksde_debug_print_string_value(
			     function,
			     "volume identifier\t\t\t",
			     ( (luksde_volume_header_v2_t *) data )->volume_identifier,
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
			if( libluksde_debug_print_string_value(
			     function,
			     "subsystem\t\t\t\t",
			     ( (luksde_volume_header_v2_t *) data )->subsystem,
			     48,
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
			byte_stream_copy_to_uint64_big_endian(
			 ( (luksde_volume_header_v2_t *) data )->header_offset,
			 value_64bit );
			libcnotify_printf(
			 "%s: header offset\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: padding1:\n",
			 function );
			libcnotify_print_data(
			 ( (luksde_volume_header_v2_t *) data )->padding1,
			 184,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			libcnotify_printf(
			 "%s: checksum:\n",
			 function );
			libcnotify_print_data(
			 ( (luksde_volume_header_v2_t *) data )->checksum,
			 64,
			 0 );

			libcnotify_printf(
			 "%s: padding2:\n",
			 function );
			libcnotify_print_data(
			 ( (luksde_volume_header_v2_t *) data )->padding2,
			 3584,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( volume_header->format_version == 1 )
	{
		if( libluksde_volume_parse_encryption_method(
		     volume_header,
		     ( (luksde_volume_header_v1_t *) data )->encryption_method,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to parse encryption method.",
			 function );

			goto on_error;
		}
		if( libluksde_volume_parse_encryption_mode(
		     volume_header,
		     ( (luksde_volume_header_v1_t *) data )->encryption_mode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to parse encryption mode.",
			 function );

			goto on_error;
		}
		if( libluksde_volume_parse_hashing_method(
		     volume_header,
		     ( (luksde_volume_header_v1_t *) data )->hashing_method,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to parse hashing method.",
			 function );

			goto on_error;
		}
		data_offset = sizeof( luksde_volume_header_v1_t );

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
			     sizeof( luksde_volume_keyslot_t ),
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

			data_offset += sizeof( luksde_volume_keyslot_t );
		}
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
	uint8_t volume_header_data[ 4096 ];

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
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              volume_header_data,
	              4096,
	              file_offset,
	              error );

	if( read_count != (ssize_t) 4096 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libluksde_volume_header_read_data(
	     volume_header,
	     volume_header_data,
	     4096,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
int libluksde_volume_header_get_format_version(
     libluksde_volume_header_t *volume_header,
     uint16_t *format_version,
     libcerror_error_t **error )
{
	static char *function = "libluksde_volume_header_get_format_version";

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
	if( format_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format version.",
		 function );

		return( -1 );
	}
	*format_version = volume_header->format_version;

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

