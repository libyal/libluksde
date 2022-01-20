/*
 * The Linux Unified Key Setup (LUKS) keyslot definitions
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

#if !defined( _LUKSDE_KEYSLOT_H )
#define _LUKSDE_KEYSLOT_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct luksde_volume_keyslot luksde_volume_keyslot_t;

struct luksde_volume_keyslot
{
	/* The state
	 * Consists of 4 bytes
	 */
	uint8_t state[ 4 ];

	/* The number of iterations
	 * Consists of 4 bytes
	 */
	uint8_t number_of_iterations[ 4 ];

	/* The salt
	 * Consists of 32 bytes
	 */
	uint8_t salt[ 32 ];

	/* The start sector
	 * Consists of 4 bytes
	 */
	uint8_t start_sector[ 4 ];

	/* The number of stripes
	 * Consists of 4 bytes
	 */
	uint8_t number_of_stripes[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LUKSDE_KEYSLOT_H ) */
