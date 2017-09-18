/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBLUKSDE_IO_HANDLE_H )
#define _LIBLUKSDE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libluksde_encryption.h"
#include "libluksde_libbfio.h"
#include "libluksde_libcdata.h"
#include "libluksde_libcerror.h"
#include "libluksde_libfcache.h"
#include "libluksde_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t luksde_signature[ 6 ];

typedef struct libluksde_io_handle libluksde_io_handle_t;

struct libluksde_io_handle
{
	/* The (format) version
	 */
	uint16_t version;

	/* The bytes per sector
	 */
	uint16_t bytes_per_sector;

	/* The volume size
	 */
	size64_t volume_size;

	/* The encrypted volume offset
	 */
	off64_t encrypted_volume_offset;

	/* The encrypted volume size
	 */
	size64_t encrypted_volume_size;

	/* Encryption method
	 */
	int encryption_method;

	/* Encryption chaining mode
	 */
	int encryption_chaining_mode;

	/* Initialization vector mode
	 */
	int initialization_vector_mode;

	/* The password and diffuser hashing method
	 */
	int hashing_method;

	/* The ESSIV hashing method
	 */
	int essiv_hashing_method;

	/* The master key
	 */
	uint8_t master_key[ 32 ];

	/* The master key size
	 */
	uint32_t master_key_size;

	/* The master key validation hash
	 */
	uint8_t master_key_validation_hash[ 20 ];

	/* The master key salt
	 */
	uint8_t master_key_salt[ 32 ];

	/* The master key number of iterations
	 */
	uint32_t master_key_number_of_iterations;

	/* The encryption context
	 */
	libluksde_encryption_context_t *encryption_context;

	/* The key slots array
	 */
	libcdata_array_t *key_slots_array;

	/* The user password
	 */
	uint8_t *user_password;

        /* User password size
	 */
	size_t user_password_size;

	/* Value to indicate the user password is set
	 */
	uint8_t user_password_is_set;

	/* Value to indicate the keys are set
	 */
	uint8_t keys_are_set;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libluksde_io_handle_initialize(
     libluksde_io_handle_t **io_handle,
     libcerror_error_t **error );

int libluksde_io_handle_free(
     libluksde_io_handle_t **io_handle,
     libcerror_error_t **error );

int libluksde_io_handle_clear(
     libluksde_io_handle_t *io_handle,
     libcerror_error_t **error );

int libluksde_io_handle_read_volume_header(
     libluksde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libluksde_io_handle_read_sector(
     libluksde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_IO_HANDLE_H ) */

