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

#if !defined( _LIBLUKSDE_VOLUME_HEADER_H )
#define _LIBLUKSDE_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#include "libluksde_libbfio.h"
#include "libluksde_libcdata.h"
#include "libluksde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libluksde_volume_header libluksde_volume_header_t;

struct libluksde_volume_header
{
	/* The format version
	 */
	uint16_t format_version;

	/* The encrypted volume start sector
	 */
	uint64_t encrypted_volume_start_sector;

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

	/* The volume identifier
	 */
	uint8_t volume_identifier[ 16 ];

	/* The key slots array
	 */
	libcdata_array_t *key_slots_array;
};

int libluksde_volume_header_initialize(
     libluksde_volume_header_t **volume_header,
     libcerror_error_t **error );

int libluksde_volume_header_free(
     libluksde_volume_header_t **volume_header,
     libcerror_error_t **error );

int libluksde_volume_parse_encryption_method(
     libluksde_volume_header_t *volume_header,
     const uint8_t *encryption_method,
     libcerror_error_t **error );

int libluksde_volume_parse_encryption_mode(
     libluksde_volume_header_t *volume_header,
     const uint8_t *encryption_mode,
     libcerror_error_t **error );

int libluksde_volume_parse_hashing_method(
     libluksde_volume_header_t *volume_header,
     const uint8_t *hashing_method,
     libcerror_error_t **error );

int libluksde_volume_header_read_data(
     libluksde_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libluksde_volume_header_read_file_io_handle(
     libluksde_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libluksde_volume_header_get_format_version(
     libluksde_volume_header_t *volume_header,
     uint16_t *format_version,
     libcerror_error_t **error );

int libluksde_volume_header_get_volume_identifier(
     libluksde_volume_header_t *volume_header,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_VOLUME_HEADER_H ) */

