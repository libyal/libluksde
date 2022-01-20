/*
 * Key slot functions
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

#if !defined( _LIBLUKSDE_KEY_SLOT_H )
#define _LIBLUKSDE_KEY_SLOT_H

#include <common.h>
#include <types.h>

#include "libluksde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libluksde_key_slot libluksde_key_slot_t;

struct libluksde_key_slot
{
	/* The state
	 */
	uint32_t state;

	/* The key material offset
	 */
	off64_t key_material_offset;

	/* The salt
	 */
	uint8_t salt[ 32 ];

	/* The number of iterations
	 */
	uint32_t number_of_iterations;

	/* The number of stripes
	 */
	uint32_t number_of_stripes;
};

int libluksde_key_slot_initialize(
     libluksde_key_slot_t **key_slot,
     libcerror_error_t **error );

int libluksde_key_slot_free(
     libluksde_key_slot_t **key_slot,
     libcerror_error_t **error );

int libluksde_key_slot_read_data(
     libluksde_key_slot_t *key_slot,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_KEY_SLOT_H ) */

