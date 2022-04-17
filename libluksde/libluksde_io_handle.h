/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBLUKSDE_IO_HANDLE_H )
#define _LIBLUKSDE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libluksde_libbfio.h"
#include "libluksde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t luksde_signature[ 6 ];

typedef struct libluksde_io_handle libluksde_io_handle_t;

struct libluksde_io_handle
{
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

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_IO_HANDLE_H ) */

