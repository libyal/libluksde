/*
 * Sector data vector functions
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

#if !defined( _LIBLUKSDE_SECTOR_DATA_VECTOR_H )
#define _LIBLUKSDE_SECTOR_DATA_VECTOR_H

#include <common.h>
#include <types.h>

#include "libluksde_encryption_context.h"
#include "libluksde_libcerror.h"
#include "libluksde_libbfio.h"
#include "libluksde_libfcache.h"
#include "libluksde_sector_data.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libluksde_sector_data_vector libluksde_sector_data_vector_t;

struct libluksde_sector_data_vector
{
	/* The cache timestamp
	 */
	int64_t cache_timestamp;

	/* The sector data cache
	 */
	libfcache_cache_t *cache;

	/* The number of bytes per sector
	 */
	uint16_t bytes_per_sector;

	/* The data offset
	 */
	off64_t data_offset;

	/* The data size
	 */
	size64_t data_size;
};

int libluksde_sector_data_vector_initialize(
     libluksde_sector_data_vector_t **sector_data_vector,
     uint16_t bytes_per_sector,
     off64_t data_offset,
     size64_t data_size,
     libcerror_error_t **error );

int libluksde_sector_data_vector_free(
     libluksde_sector_data_vector_t **sector_data_vector,
     libcerror_error_t **error );

int libluksde_sector_data_vector_get_sector_data_at_offset(
     libluksde_sector_data_vector_t *sector_data_vector,
     libbfio_handle_t *file_io_handle,
     libluksde_encryption_context_t *encryption_context,
     off64_t offset,
     libluksde_sector_data_t **sector_data,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_SECTOR_DATA_VECTOR_H ) */

