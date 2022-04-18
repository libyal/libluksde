/*
 * Encryption functions
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
#include "libluksde_encryption_context.h"
#include "libluksde_libcaes.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"
#include "libluksde_libfcrypto.h"
#include "libluksde_libhmac.h"

/* Creates an encryption context
 * Make sure the value context is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libluksde_encryption_context_initialize(
     libluksde_encryption_context_t **context,
     int method,
     int chaining_mode,
     int initialization_vector_mode,
     int essiv_hashing_method,
     libcerror_error_t **error )
{
	static char *function = "libluksde_encryption_context_initialize";
	int encryption_mode   = 0;
	int result            = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid context value already set.",
		 function );

		return( -1 );
	}
	if( method == LIBLUKSDE_ENCRYPTION_METHOD_AES )
	{
		if( chaining_mode == LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC )
		{
			encryption_mode = LIBLUKSDE_ENCRYPTION_MODE_AES_CBC;
		}
		else if( chaining_mode == LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB )
		{
			encryption_mode = LIBLUKSDE_ENCRYPTION_MODE_AES_ECB;
		}
		else if( chaining_mode == LIBLUKSDE_ENCRYPTION_CHAINING_MODE_XTS )
		{
			encryption_mode = LIBLUKSDE_ENCRYPTION_MODE_AES_XTS;
		}
	}
	else if( method == LIBLUKSDE_ENCRYPTION_METHOD_ARC4 )
	{
		if( chaining_mode == LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC )
		{
			encryption_mode = LIBLUKSDE_ENCRYPTION_MODE_RC4_CBC;
		}
		else if( chaining_mode == LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB )
		{
			encryption_mode = LIBLUKSDE_ENCRYPTION_MODE_RC4_ECB;
		}
	}
	else if( method == LIBLUKSDE_ENCRYPTION_METHOD_SERPENT )
	{
		if( chaining_mode == LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC )
		{
			encryption_mode = LIBLUKSDE_ENCRYPTION_MODE_SERPENT_CBC;
		}
		else if( chaining_mode == LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB )
		{
			encryption_mode = LIBLUKSDE_ENCRYPTION_MODE_SERPENT_ECB;
		}
	}
	if( encryption_mode == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported method and chaining mode.",
		 function );

		return( -1 );
	}
	*context = memory_allocate_structure(
	            libluksde_encryption_context_t );

	if( *context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *context,
	     0,
	     sizeof( libluksde_encryption_context_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		memory_free(
		 *context );

		*context = NULL;

		return( -1 );
	}
	switch( encryption_mode )
	{
		case LIBLUKSDE_ENCRYPTION_MODE_AES_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_AES_ECB:
			result = libcaes_context_initialize(
			          &( ( *context )->aes_decryption_context ),
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_AES_XTS:
			result = libcaes_tweaked_context_initialize(
			          &( ( *context )->aes_xts_decryption_context ),
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_RC4_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_RC4_ECB:
			result = libfcrypto_rc4_context_initialize(
			          &( ( *context )->rc4_decryption_context ),
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_ECB:
			result = libfcrypto_serpent_context_initialize(
			          &( ( *context )->serpent_decryption_context ),
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
		 "%s: unable to initialize decryption context.",
		 function );

		goto on_error;
	}
	switch( encryption_mode )
	{
		case LIBLUKSDE_ENCRYPTION_MODE_AES_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_AES_ECB:
			result = libcaes_context_initialize(
			          &( ( *context )->aes_encryption_context ),
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_AES_XTS:
			result = libcaes_tweaked_context_initialize(
			          &( ( *context )->aes_xts_encryption_context ),
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_RC4_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_RC4_ECB:
			result = libfcrypto_rc4_context_initialize(
			          &( ( *context )->rc4_encryption_context ),
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_ECB:
			result = libfcrypto_serpent_context_initialize(
			          &( ( *context )->serpent_encryption_context ),
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
		 "%s: unable to initialize encryption context.",
		 function );

		goto on_error;
	}
	if( initialization_vector_mode == LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV )
	{
		switch( encryption_mode )
		{
			case LIBLUKSDE_ENCRYPTION_MODE_AES_CBC:
			case LIBLUKSDE_ENCRYPTION_MODE_AES_ECB:
				result = libcaes_context_initialize(
					  &( ( *context )->essiv_encryption_context ),
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
			 "%s: unable to initialize ESSIV encryption context.",
			 function );

			goto on_error;
		}
	}
	( *context )->encryption_mode            = encryption_mode;
	( *context )->initialization_vector_mode = initialization_vector_mode;
	( *context )->essiv_hashing_method       = essiv_hashing_method;

	return( 1 );

on_error:
	if( *context != NULL )
	{
		if( ( *context )->serpent_encryption_context != NULL )
		{
			libfcrypto_serpent_context_free(
			 &( ( *context )->serpent_encryption_context ),
			 NULL );
		}
		if( ( *context )->serpent_decryption_context != NULL )
		{
			libfcrypto_serpent_context_free(
			 &( ( *context )->serpent_decryption_context ),
			 NULL );
		}
		if( ( *context )->rc4_encryption_context != NULL )
		{
			libfcrypto_rc4_context_free(
			 &( ( *context )->rc4_encryption_context ),
			 NULL );
		}
		if( ( *context )->rc4_decryption_context != NULL )
		{
			libfcrypto_rc4_context_free(
			 &( ( *context )->rc4_decryption_context ),
			 NULL );
		}
		if( ( *context )->aes_xts_encryption_context != NULL )
		{
			libcaes_tweaked_context_free(
			 &( ( *context )->aes_xts_encryption_context ),
			 NULL );
		}
		if( ( *context )->aes_xts_decryption_context != NULL )
		{
			libcaes_tweaked_context_free(
			 &( ( *context )->aes_xts_decryption_context ),
			 NULL );
		}
		if( ( *context )->aes_encryption_context != NULL )
		{
			libcaes_context_free(
			 &( ( *context )->aes_encryption_context ),
			 NULL );
		}
		if( ( *context )->aes_decryption_context != NULL )
		{
			libcaes_context_free(
			 &( ( *context )->aes_decryption_context ),
			 NULL );
		}
		memory_free(
		 *context );

		*context = NULL;
	}
	return( -1 );
}

/* Frees an encryption context
 * Returns 1 if successful or -1 on error
 */
int libluksde_encryption_context_free(
     libluksde_encryption_context_t **context,
     libcerror_error_t **error )
{
	static char *function = "libluksde_encryption_context_free";
	int result            = 1;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		if( ( *context )->aes_decryption_context != NULL )
		{
			if( libcaes_context_free(
			     &( ( *context )->aes_decryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free AES decryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->aes_encryption_context != NULL )
		{
			if( libcaes_context_free(
			     &( ( *context )->aes_encryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free AES encryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->aes_xts_decryption_context != NULL )
		{
			if( libcaes_tweaked_context_free(
			     &( ( *context )->aes_xts_decryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free AES-XTS decryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->aes_xts_encryption_context != NULL )
		{
			if( libcaes_tweaked_context_free(
			     &( ( *context )->aes_xts_encryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free AES-XTS encryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->rc4_decryption_context != NULL )
		{
			if( libfcrypto_rc4_context_free(
			     &( ( *context )->rc4_decryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free RC4 decryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->rc4_encryption_context != NULL )
		{
			if( libfcrypto_rc4_context_free(
			     &( ( *context )->rc4_encryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free RC4 encryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->serpent_decryption_context != NULL )
		{
			if( libfcrypto_serpent_context_free(
			     &( ( *context )->serpent_decryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free Serpent decryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->serpent_encryption_context != NULL )
		{
			if( libfcrypto_serpent_context_free(
			     &( ( *context )->serpent_encryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free Serpent encryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->essiv_encryption_context != NULL )
		{
			if( libcaes_context_free(
			     &( ( *context )->essiv_encryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free ESSIV encryption context.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *context );

		*context = NULL;
	}
	return( result );
}

/* Sets the de- and encryption key
 * Returns 1 if successful or -1 on error
 */
int libluksde_encryption_context_set_key(
     libluksde_encryption_context_t *context,
     const uint8_t *key,
     size_t key_size,
     libcerror_error_t **error )
{
	uint8_t essiv_key[ 32 ];

	static char *function = "libluksde_encryption_context_set_key";
	size_t key_bit_size   = 0;
	int result            = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	key_bit_size = key_size * 8;

	switch( context->encryption_mode )
	{
		case LIBLUKSDE_ENCRYPTION_MODE_AES_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_AES_ECB:
			result = libcaes_context_set_key(
			          context->aes_decryption_context,
			          LIBCAES_CRYPT_MODE_DECRYPT,
			          key,
			          key_bit_size,
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_AES_XTS:
			key_bit_size /= 2;
			key_size     /= 2;

			result = libcaes_tweaked_context_set_keys(
			          context->aes_xts_decryption_context,
			          LIBCAES_CRYPT_MODE_DECRYPT,
			          key,
			          key_bit_size,
			          &( key[ key_size ] ),
			          key_bit_size,
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_RC4_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_RC4_ECB:
			result = libfcrypto_rc4_context_set_key(
			          context->rc4_decryption_context,
			          key,
			          key_bit_size,
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_ECB:
			result = libfcrypto_serpent_context_set_key(
			          context->serpent_decryption_context,
			          key,
			          key_bit_size,
			          error );
			break;

		default:
			break;
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set key in decryption context.",
		 function );

		goto on_error;
	}
	switch( context->encryption_mode )
	{
		case LIBLUKSDE_ENCRYPTION_MODE_AES_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_AES_ECB:
			result = libcaes_context_set_key(
			          context->aes_encryption_context,
			          LIBCAES_CRYPT_MODE_ENCRYPT,
			          key,
			          key_bit_size,
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_AES_XTS:
			result = libcaes_tweaked_context_set_keys(
			          context->aes_xts_encryption_context,
			          LIBCAES_CRYPT_MODE_ENCRYPT,
			          key,
			          key_bit_size,
			          &( key[ key_size ] ),
			          key_bit_size,
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_RC4_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_RC4_ECB:
			result = libfcrypto_rc4_context_set_key(
			          context->rc4_encryption_context,
			          key,
			          key_bit_size,
			          error );
			break;

		case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_CBC:
		case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_ECB:
			result = libfcrypto_serpent_context_set_key(
			          context->serpent_encryption_context,
			          key,
			          key_bit_size,
			          error );
			break;

		default:
			break;
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set key in encryption context.",
		 function );

		goto on_error;
	}
	if( context->initialization_vector_mode == LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV )
	{
		if( memory_set(
		     essiv_key,
		     0,
		     32 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear ESSIV key.",
			 function );

			goto on_error;
		}
		switch( context->essiv_hashing_method )
		{
			case LIBLUKSDE_HASHING_METHOD_SHA1:
	                	result = libhmac_sha1_calculate(
				          key,
				          key_size,
				          essiv_key,
				          32,
				          error );
				break;

			case LIBLUKSDE_HASHING_METHOD_SHA256:
	                	result = libhmac_sha256_calculate(
				          key,
				          key_size,
				          essiv_key,
				          32,
				          error );
				break;

			default:
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compute ESSIV encryption key.",
			 function );

			goto on_error;
		}
		switch( context->encryption_mode )
		{
			case LIBLUKSDE_ENCRYPTION_MODE_AES_CBC:
			case LIBLUKSDE_ENCRYPTION_MODE_AES_ECB:
				result = libcaes_context_set_key(
					  context->essiv_encryption_context,
					  LIBCAES_CRYPT_MODE_ENCRYPT,
					  essiv_key,
					  key_bit_size,
					  error );
				break;

			default:
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set key in ESSIV encryption context.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     essiv_key,
		     0,
		     32 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear ESSIV key.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( context->initialization_vector_mode == LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV )
	{
		memory_set(
		 essiv_key,
		 0,
		 32 );
	}
	return( -1 );
}

/* De- or encrypts a block of data
 * Returns 1 if successful or -1 on error
 */
int libluksde_encryption_context_crypt(
     libluksde_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t sector_number,
     libcerror_error_t **error )
{
	uint8_t block_key_data[ 16 ];
	uint8_t initialization_vector[ 16 ];

	static char *function = "libluksde_encryption_context_crypt";
	size_t data_offset    = 0;
	uint64_t block_key    = 0;
	int result            = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( ( mode != LIBLUKSDE_ENCRYPTION_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBLUKSDE_ENCRYPTION_CRYPT_MODE_ENCRYPT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( ( input_data_size < 16 )
	 || ( input_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: input data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( output_data_size < input_data_size )
	 || ( output_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: output data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     initialization_vector,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear initialization vector.",
		 function );

		return( -1 );
	}
	switch( context->initialization_vector_mode )
	{
		case LIBLUKSDE_INITIALIZATION_VECTOR_MODE_BENBI:
			block_key = ( sector_number << 5 ) + 1;

			byte_stream_copy_from_uint64_big_endian(
			 &( initialization_vector[ 8 ] ),
			 block_key );

			break;

		case LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV:
			if( memory_set(
			     block_key_data,
			     0,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear block key data.",
				 function );

				return( -1 );
			}
			byte_stream_copy_from_uint64_little_endian(
			 block_key_data,
			 sector_number );

			/* The block key for the initialization vector is encrypted
			 * with the hash of the key
			 */
			if( libcaes_crypt_ecb(
			     context->essiv_encryption_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     block_key_data,
			     16,
			     initialization_vector,
			     16,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to encrypt initialization vector.",
				 function );

				return( -1 );
			}
			break;

		case LIBLUKSDE_INITIALIZATION_VECTOR_MODE_NONE:
		case LIBLUKSDE_INITIALIZATION_VECTOR_MODE_NULL:
			break;

		case LIBLUKSDE_INITIALIZATION_VECTOR_MODE_PLAIN32:
			byte_stream_copy_from_uint32_little_endian(
			 initialization_vector,
			 (uint32_t) sector_number );

			break;

		case LIBLUKSDE_INITIALIZATION_VECTOR_MODE_PLAIN64:
			byte_stream_copy_from_uint64_little_endian(
			 initialization_vector,
			 sector_number );

			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported initialization vector mode.",
			 function );

			return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: initialization vector data:\n",
		 function );
		libcnotify_print_data(
		 initialization_vector,
		 16,
		 0 );
	}
#endif
	if( mode == LIBLUKSDE_ENCRYPTION_CRYPT_MODE_ENCRYPT )
	{
		switch( context->encryption_mode )
		{
			case LIBLUKSDE_ENCRYPTION_MODE_AES_CBC:
				result = libcaes_crypt_cbc(
					  context->aes_encryption_context,
					  LIBCAES_CRYPT_MODE_ENCRYPT,
					  initialization_vector,
					  16,
					  input_data,
					  input_data_size,
					  output_data,
					  output_data_size,
					  error );
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_AES_ECB:
				while( data_offset < input_data_size )
				{
					/* The libcaes_crypt_ecb function encrypts 16 bytes at a time
					 */
					result = libcaes_crypt_ecb(
						  context->aes_encryption_context,
						  LIBCAES_CRYPT_MODE_ENCRYPT,
						  &( input_data[ data_offset ] ),
						  input_data_size - data_offset,
						  &( output_data[ data_offset ] ),
						  output_data_size - data_offset,
						  error );

					if( result != 1 )
					{
						break;
					}
					data_offset += 16;
				}
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_AES_XTS:
				result = libcaes_crypt_xts(
					  context->aes_xts_encryption_context,
					  LIBCAES_CRYPT_MODE_ENCRYPT,
					  initialization_vector,
					  16,
					  input_data,
					  input_data_size,
					  output_data,
					  output_data_size,
					  error );
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_RC4_CBC:
			case LIBLUKSDE_ENCRYPTION_MODE_RC4_ECB:
				result = libfcrypto_rc4_crypt(
					  context->rc4_encryption_context,
					  &( input_data[ data_offset ] ),
					  input_data_size - data_offset,
					  &( output_data[ data_offset ] ),
					  output_data_size - data_offset,
					  error );

				if( result != 1 )
				{
					break;
				}
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_CBC:
			case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_ECB:
				while( data_offset < input_data_size )
				{
					/* The libfcrypto_serpent_crypt_ecb function encrypts 16 bytes at a time
					 */
					result = libfcrypto_serpent_crypt_ecb(
						  context->serpent_encryption_context,
						  LIBFCRYPTO_SERPENT_CRYPT_MODE_ENCRYPT,
						  &( input_data[ data_offset ] ),
						  input_data_size - data_offset,
						  &( output_data[ data_offset ] ),
						  output_data_size - data_offset,
						  error );

					if( result != 1 )
					{
						break;
					}
					data_offset += 16;
				}
				break;

			default:
				result = 0;
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to encrypt output data.",
			 function );

			return( -1 );
		}
	}
	else
	{
		switch( context->encryption_mode )
		{
			case LIBLUKSDE_ENCRYPTION_MODE_AES_CBC:
				result = libcaes_crypt_cbc(
					  context->aes_decryption_context,
					  LIBCAES_CRYPT_MODE_DECRYPT,
					  initialization_vector,
					  16,
					  input_data,
					  input_data_size,
					  output_data,
					  output_data_size,
					  error );
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_AES_ECB:
				while( data_offset < input_data_size )
				{
					/* The libcaes_crypt_ecb function decrypts 16 bytes at a time
					 */
					result = libcaes_crypt_ecb(
						  context->aes_decryption_context,
						  LIBCAES_CRYPT_MODE_DECRYPT,
						  &( input_data[ data_offset ] ),
						  input_data_size - data_offset,
						  &( output_data[ data_offset ] ),
						  output_data_size - data_offset,
						  error );

					if( result != 1 )
					{
						break;
					}
					data_offset += 16;
				}
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_AES_XTS:
				result = libcaes_crypt_xts(
					  context->aes_xts_decryption_context,
					  LIBCAES_CRYPT_MODE_DECRYPT,
					  initialization_vector,
					  16,
					  input_data,
					  input_data_size,
					  output_data,
					  output_data_size,
					  error );
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_RC4_CBC:
			case LIBLUKSDE_ENCRYPTION_MODE_RC4_ECB:
				result = libfcrypto_rc4_crypt(
					  context->rc4_decryption_context,
					  input_data,
					  input_data_size,
					  output_data,
					  output_data_size,
					  error );

				if( result != 1 )
				{
					break;
				}
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_CBC:
				result = libfcrypto_serpent_crypt_cbc(
					  context->serpent_decryption_context,
					  LIBFCRYPTO_SERPENT_CRYPT_MODE_DECRYPT,
					  initialization_vector,
					  16,
					  input_data,
					  input_data_size,
					  output_data,
					  output_data_size,
					  error );

				if( result != 1 )
				{
					break;
				}
				break;

			case LIBLUKSDE_ENCRYPTION_MODE_SERPENT_ECB:
				while( data_offset < input_data_size )
				{
					/* The libfcrypto_serpent_crypt_ecb function decrypts 16 bytes at a time
					 */
					result = libfcrypto_serpent_crypt_ecb(
						  context->serpent_decryption_context,
						  LIBFCRYPTO_SERPENT_CRYPT_MODE_DECRYPT,
						  &( input_data[ data_offset ] ),
						  input_data_size - data_offset,
						  &( output_data[ data_offset ] ),
						  output_data_size - data_offset,
						  error );

					if( result != 1 )
					{
						break;
					}
					data_offset += 16;
				}
				break;

			default:
				result = 0;
				break;
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to decrypt output data.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

