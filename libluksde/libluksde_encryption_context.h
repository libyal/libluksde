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

#if !defined( _LIBLUKSDE_ENCRYPTION_H )
#define _LIBLUKSDE_ENCRYPTION_H

#include <common.h>
#include <types.h>

#include "libluksde_libcaes.h"
#include "libluksde_libcerror.h"
#include "libluksde_libfcrypto.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBLUKSDE_ENCRYPTION_CRYPT_MODES
{
	LIBLUKSDE_ENCRYPTION_CRYPT_MODE_DECRYPT   = 0,
	LIBLUKSDE_ENCRYPTION_CRYPT_MODE_ENCRYPT   = 1
};

typedef struct libluksde_encryption_context libluksde_encryption_context_t;

struct libluksde_encryption_context
{
	/* The encryption mode, which is a combination of method and chaining mode
	 */
	int encryption_mode;

	/* The initialization vector mode
	 */
	int initialization_vector_mode;

	/* The ESSIV hashing method
	 */
	int essiv_hashing_method;

	/* The AES decryption context
	 */
	libcaes_context_t *aes_decryption_context;

	/* The AES encryption context
	 */
	libcaes_context_t *aes_encryption_context;

	/* The AES-XTS decryption context
	 */
	libcaes_tweaked_context_t *aes_xts_decryption_context;

	/* The AES-XTS encryption context
	 */
	libcaes_tweaked_context_t *aes_xts_encryption_context;

	/* The RC4 decryption context
	 */
	libfcrypto_rc4_context_t *rc4_decryption_context;

	/* The RC4 encryption context
	 */
	libfcrypto_rc4_context_t *rc4_encryption_context;

	/* The Serpent decryption context
	 */
	libfcrypto_serpent_context_t *serpent_decryption_context;

	/* The Serpent encryption context
	 */
	libfcrypto_serpent_context_t *serpent_encryption_context;

	/* The ESSIV encryption context
	 */
	libcaes_context_t *essiv_encryption_context;
};

int libluksde_encryption_context_initialize(
     libluksde_encryption_context_t **context,
     int method,
     int chaining_mode,
     int initialization_vector_mode,
     int essiv_hashing_method,
     libcerror_error_t **error );

int libluksde_encryption_context_free(
     libluksde_encryption_context_t **context,
     libcerror_error_t **error );

int libluksde_encryption_context_set_key(
     libluksde_encryption_context_t *context,
     const uint8_t *key,
     size_t key_size,
     libcerror_error_t **error );

int libluksde_encryption_context_crypt(
     libluksde_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t sector_number,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_ENCRYPTION_H ) */

