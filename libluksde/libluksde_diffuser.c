/*
 * LUKS anti-forensic (AF) diffuser functions
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

#include "libluksde_definitions.h"
#include "libluksde_diffuser.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"
#include "libluksde_libhmac.h"

/* Diffuses data
 * Returns 1 if successful or -1 on error
 */
int libluksde_diffuser_diffuse(
     uint8_t *data,
     size_t data_size,
     int hashing_method,
     libcerror_error_t **error )
{
	uint8_t block_index_buffer[ 4 ];
	uint8_t hash_buffer[ 64 ];

	libhmac_sha1_context_t *sha1_context     = NULL;
	libhmac_sha224_context_t *sha224_context = NULL;
	libhmac_sha256_context_t *sha256_context = NULL;
	libhmac_sha512_context_t *sha512_context = NULL;
	static char *function                    = "libluksde_diffuser_diffuse";
	size_t data_offset                       = 0;
	size_t hash_size                         = 0;
	size_t read_size                         = 0;
	uint32_t block_index                     = 0;
	int result                               = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T <= 4
	if( data_size > (size_t) SSIZE_MAX )
#else
	if( data_size > (size_t) UINT32_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	switch( hashing_method )
	{
		case LIBLUKSDE_HASHING_METHOD_SHA1:
			hash_size = LIBHMAC_SHA1_HASH_SIZE;
			break;

		case LIBLUKSDE_HASHING_METHOD_SHA224:
			hash_size = LIBHMAC_SHA224_HASH_SIZE;
			break;

		case LIBLUKSDE_HASHING_METHOD_SHA256:
			hash_size = LIBHMAC_SHA256_HASH_SIZE;
			break;

		case LIBLUKSDE_HASHING_METHOD_SHA512:
			hash_size = LIBHMAC_SHA512_HASH_SIZE;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported hashing method.",
			 function );

			return( -1 );
	}
	while( data_size > 0 )
	{
		switch( hashing_method )
		{
			case LIBLUKSDE_HASHING_METHOD_SHA1:
				result = libhmac_sha1_initialize(
					  &sha1_context,
					  error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA224:
				result = libhmac_sha224_initialize(
					  &sha224_context,
					  error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA256:
				result = libhmac_sha256_initialize(
					  &sha256_context,
					  error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA512:
				result = libhmac_sha512_initialize(
					  &sha512_context,
					  error );
				break;

			default:
				result = 0;
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize context.",
			 function );

			goto on_error;
		}
		byte_stream_copy_from_uint32_big_endian(
		 block_index_buffer,
		 block_index );

		switch( hashing_method )
		{
			case LIBLUKSDE_HASHING_METHOD_SHA1:
				result = libhmac_sha1_update(
				          sha1_context,
				          block_index_buffer,
				          4,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA224:
				result = libhmac_sha224_update(
				          sha224_context,
				          block_index_buffer,
				          4,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA256:
				result = libhmac_sha256_update(
				          sha256_context,
				          block_index_buffer,
				          4,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA512:
				result = libhmac_sha512_update(
				          sha512_context,
				          block_index_buffer,
				          4,
				          error );
				break;

			default:
				result = 0;
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update context.",
			 function );

			goto on_error;
		}
		if( data_size < hash_size )
		{
			read_size = data_size;
		}
		else
		{
			read_size = hash_size;
		}
		switch( hashing_method )
		{
			case LIBLUKSDE_HASHING_METHOD_SHA1:
				result = libhmac_sha1_update(
				          sha1_context,
				          &( data[ data_offset ] ),
				          read_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA224:
				result = libhmac_sha224_update(
				          sha224_context,
				          &( data[ data_offset ] ),
				          read_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA256:
				result = libhmac_sha256_update(
				          sha256_context,
				          &( data[ data_offset ] ),
				          read_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA512:
				result = libhmac_sha512_update(
				          sha512_context,
				          &( data[ data_offset ] ),
				          read_size,
				          error );
				break;

			default:
				result = 0;
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update context.",
			 function );

			goto on_error;
		}
		switch( hashing_method )
		{
			case LIBLUKSDE_HASHING_METHOD_SHA1:
				result = libhmac_sha1_finalize(
				          sha1_context,
				          hash_buffer,
				          hash_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA224:
				result = libhmac_sha224_finalize(
				          sha224_context,
				          hash_buffer,
				          hash_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA256:
				result = libhmac_sha256_finalize(
				          sha256_context,
				          hash_buffer,
				          hash_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA512:
				result = libhmac_sha512_finalize(
				          sha512_context,
				          hash_buffer,
				          hash_size,
				          error );
				break;

			default:
				result = 0;
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize context.",
			 function );

			goto on_error;
		}
		switch( hashing_method )
		{
			case LIBLUKSDE_HASHING_METHOD_SHA1:
				result = libhmac_sha1_free(
				          &sha1_context,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA224:
				result = libhmac_sha224_free(
				          &sha224_context,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA256:
				result = libhmac_sha256_free(
				          &sha256_context,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA512:
				result = libhmac_sha512_free(
				          &sha512_context,
				          error );
				break;

			default:
				result = 0;
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free context.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     &( data[ data_offset ] ),
		     hash_buffer,
		     read_size ) == NULL)
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy hash to data.",
			 function );

			goto on_error;
		}
		data_offset += read_size;
		data_size   -= read_size;

		block_index++;
	}
	return( 1 );

on_error:
	if( sha512_context != NULL )
	{
		libhmac_sha512_free(
		 &sha512_context,
		 NULL );
	}
	if( sha256_context != NULL )
	{
		libhmac_sha256_free(
		 &sha256_context,
		 NULL );
	}
	if( sha224_context != NULL )
	{
		libhmac_sha224_free(
		 &sha224_context,
		 NULL );
	}
	if( sha1_context != NULL )
	{
		libhmac_sha1_free(
		 &sha1_context,
		 NULL );
	}
	return( -1 );
}

/* Merges the split data using the diffuser
 * Returns 1 if successful or -1 on error
 */
int libluksde_diffuser_merge(
     const uint8_t *split_data,
     size_t split_data_size,
     uint8_t *data,
     size_t data_size,
     uint32_t number_of_stripes,
     int hashing_method,
     libcerror_error_t **error )
{
	static char *function    = "libluksde_diffuser_merge";
	size_t data_offset       = 0;
	size_t split_data_offset = 0;
	uint32_t stripe_iterator = 0;

	if( split_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split data.",
		 function );

		return( -1 );
	}
	if( ( split_data_size == 0 )
	 || ( split_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid split data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > split_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( number_of_stripes < 1 )
	 || ( number_of_stripes > ( split_data_size / data_size ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of stripes value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     data,
	     0,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data.",
		 function );

		return( -1 );
	}
	for( stripe_iterator = 0;
	     stripe_iterator < ( number_of_stripes - 1 );
	     stripe_iterator++ )
	{
		for( data_offset = 0;
		     data_offset < data_size;
		     data_offset++ )
		{
			data[ data_offset ] ^= split_data[ split_data_offset ];

			split_data_offset++;
		}
		if( libluksde_diffuser_diffuse(
		     data,
		     data_size,
		     hashing_method,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to diffuse stripe: %" PRIu32 ".",
			 function,
			 stripe_iterator );

			return( -1 );
		}
	}
	for( data_offset = 0;
	     data_offset < data_size;
	     data_offset++ )
	{
		data[ data_offset ] ^= split_data[ split_data_offset ];

		split_data_offset++;
	}
	return( 1 );
}

