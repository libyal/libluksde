/*
 * Password functions
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
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"
#include "libluksde_libhmac.h"
#include "libluksde_password.h"

/* Compute a PBKDF2-derived key from the given input.
 * Returns 1 if successful or -1 on error
 */
int libluksde_password_pbkdf2(
     const uint8_t *password,
     size_t password_length,
     int password_hashing_method,
     const uint8_t *salt,
     size_t salt_size,
     uint32_t number_of_iterations,
     uint8_t *output_data,
     size_t output_data_size,
     libcerror_error_t **error )
{
	uint8_t block_buffer[ 64 ];
	uint8_t hash_buffer[ 64 ];

	uint8_t *data_buffer       = NULL;
	static char *function      = "libluksde_password_pbkdf2";
	size_t data_buffer_size    = 0;
	size_t hash_size           = 0;
	size_t output_data_offset  = 0;
	size_t remaining_data_size = 0;
	uint32_t block_index       = 0;
	uint32_t byte_index        = 0;
	uint32_t number_of_blocks  = 0;
	uint32_t password_iterator = 0;
	int result                 = 0;

	if( password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid password.",
		 function );

		return( -1 );
	}
	if( password_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid password length value exceeds maximum.",
		 function );

		return( -1 );
	}
	switch( password_hashing_method )
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
			 "%s: unsupported password hashing method.",
			 function );

			return( -1 );
	}
	if( salt == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid salt.",
		 function );

		return( -1 );
	}
	if( salt_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid salt size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( number_of_iterations == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of iterations value zero or less.",
		 function );

		return( -1 );
	}
	if( output_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output data.",
		 function );

		return( -1 );
	}
	if( output_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid output data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     block_buffer,
	     0,
	     64 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to clear hash buffer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     hash_buffer,
	     0,
	     64 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to clear hash buffer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     output_data,
	     0,
	     output_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to clear output data.",
		 function );

		goto on_error;
	}
/* TODO add bounds check */
	number_of_blocks    = output_data_size / hash_size;
	remaining_data_size = output_data_size % hash_size;

	if( remaining_data_size != 0 )
	{
		number_of_blocks += 1;
	}
	data_buffer_size = salt_size + 4;

	if( data_buffer_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data buffer size value exceeds maximum allocation size.",
		 function );

		return( -1 );
	}
	data_buffer = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * data_buffer_size );

	if( data_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data buffer.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     data_buffer,
	     salt,
	     salt_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy salt into data buffer.",
		 function );

		goto on_error;
	}
	byte_stream_copy_from_uint32_big_endian(
	 &( data_buffer[ salt_size ] ),
	 0 );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: password:\n",
		 function );
		libcnotify_print_data(
		 password,
		 password_length,
		 0 );

		libcnotify_printf(
		 "%s: data buffer:\n",
		 function );
		libcnotify_print_data(
		 data_buffer,
		 data_buffer_size,
		 0 );

		libcnotify_printf(
		 "%s: hash size\t\t\t\t\t: %" PRIzd "\n",
		 function,
		 hash_size );

		libcnotify_printf(
		 "%s: number of iterations\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_iterations );

		libcnotify_printf(
		 "%s: number of blocks\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_blocks );

		libcnotify_printf(
		 "%s: remaining data size\t\t\t\t: %" PRIzd "\n",
		 function,
		 remaining_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	for( block_index = 1;
	     block_index <= number_of_blocks;
	     block_index++ )
	{
		byte_stream_copy_from_uint32_big_endian(
		 &( data_buffer[ salt_size ] ),
		 block_index );

		switch( password_hashing_method )
		{
			case LIBLUKSDE_HASHING_METHOD_SHA1:
	                	result = libhmac_sha1_calculate_hmac(
				          password,
				          password_length,
				          data_buffer,
				          data_buffer_size,
				          hash_buffer,
				          hash_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA224:
	                	result = libhmac_sha224_calculate_hmac(
				          password,
				          password_length,
				          data_buffer,
				          data_buffer_size,
				          hash_buffer,
				          hash_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA256:
	                	result = libhmac_sha256_calculate_hmac(
				          password,
				          password_length,
				          data_buffer,
				          data_buffer_size,
				          hash_buffer,
				          hash_size,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA512:
	                	result = libhmac_sha512_calculate_hmac(
				          password,
				          password_length,
				          data_buffer,
				          data_buffer_size,
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
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compute initial hmac for block %d.",
			 function,
			 block_index );

			goto on_error;
		}
		if( memory_copy(
		     block_buffer,
		     hash_buffer,
		     hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy hash buffer into block buffer.",
			 function );

			goto on_error;
		}
		for( password_iterator = 1;
		     password_iterator < number_of_iterations;
		     password_iterator++ )
		{
			switch( password_hashing_method )
			{
				case LIBLUKSDE_HASHING_METHOD_SHA1:
					result = libhmac_sha1_calculate_hmac(
					          password,
					          password_length,
					          hash_buffer,
					          hash_size,
					          hash_buffer,
					          hash_size,
					          error );
					break;

				case LIBLUKSDE_HASHING_METHOD_SHA224:
					result = libhmac_sha224_calculate_hmac(
					          password,
					          password_length,
					          hash_buffer,
					          hash_size,
					          hash_buffer,
					          hash_size,
					          error );
					break;

				case LIBLUKSDE_HASHING_METHOD_SHA256:
					result = libhmac_sha256_calculate_hmac(
					          password,
					          password_length,
					          hash_buffer,
					          hash_size,
					          hash_buffer,
					          hash_size,
					          error );
					break;

				case LIBLUKSDE_HASHING_METHOD_SHA512:
					result = libhmac_sha512_calculate_hmac(
					          password,
					          password_length,
					          hash_buffer,
					          hash_size,
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
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compute initial hmac for block %d.",
				 function,
				 block_index );

				goto on_error;
			}
			for( byte_index = 0;
			     byte_index < hash_size;
			     byte_index++ )
			{
				block_buffer[ byte_index ] ^= hash_buffer[ byte_index ];
			}
		}
		if( hash_size > output_data_size )
		{
			hash_size = output_data_size;
		}
		if( memory_copy(
		     &( output_data[ output_data_offset ] ),
		     block_buffer,
		     hash_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy block buffer into output data.",
			 function );

			goto on_error;
		}
		output_data_offset += hash_size;
		output_data_size   -= hash_size;
	}
	if( data_buffer != NULL )
	{
		memory_free(
		 data_buffer );
	}
	return( 1 );

on_error:
	if( data_buffer != NULL )
	{
		memory_free(
		 data_buffer );
	}
	return( -1 );
}

