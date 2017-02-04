/*
 * Encryption functions
 *
 * Copyright (C) 2013-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBLUKSDE_ENCRYPTION_H )
#define _LIBLUKSDE_ENCRYPTION_H

#include <common.h>
#include <types.h>

#include "libluksde_libcaes.h"
#include "libluksde_libcerror.h"

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
	/* The (encryption) method
	 */
	int method;

	/* The (encryption) chaining mode
	 */
	int chaining_mode;

	/* The initialization vector mode
	 */
	int initialization_vector_mode;

	/* The ESSIV hashing method
	 */
	int essiv_hashing_method;

	/* The decryption context
	 */
	libcaes_context_t *decryption_context;

	/* The encryption context
	 */
	libcaes_context_t *encryption_context;

	/* The ESSIV encryption context
	 */
	libcaes_context_t *essiv_encryption_context;
};

int libluksde_encryption_initialize(
     libluksde_encryption_context_t **context,
     int method,
     int chaining_mode,
     int initialization_vector_mode,
     int essiv_hashing_method,
     libcerror_error_t **error );

int libluksde_encryption_free(
     libluksde_encryption_context_t **context,
     libcerror_error_t **error );

int libluksde_encryption_set_keys(
     libluksde_encryption_context_t *context,
     const uint8_t *key,
     size_t key_size,
     libcerror_error_t **error );

int libluksde_encryption_crypt(
     libluksde_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t block_key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_ENCRYPTION_H ) */

