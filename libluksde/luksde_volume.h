/*
 * The Linux Unified Key Setup (LUKS) volume definitions
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

#if !defined( _LUKSDE_VOLUME_H )
#define _LUKSDE_VOLUME_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct luksde_volume_header_v1 luksde_volume_header_v1_t;

struct luksde_volume_header_v1
{
	/* The signature
	 * Consists of 6 bytes
	 * Contains: LUKS\xba\xbe
	 */
	uint8_t signature[ 6 ];

	/* The format version
	 * Consists of 2 bytes
	 * Contains: 1
	 */
	uint8_t format_version[ 2 ];

	/* The encryption method
	 * Consists of 32 bytes
	 * Contains: an ASCII string with an end-of-string character
	 */
	uint8_t encryption_method[ 32 ];

	/* The encryption mode
	 * Consists of 32 bytes
	 * Contains: an ASCII string with an end-of-string character
	 */
	uint8_t encryption_mode[ 32 ];

	/* The hashing method
	 * Consists of 32 bytes
	 * Contains: an ASCII string with an end-of-string character
	 */
	uint8_t hashing_method[ 32 ];

	/* The encrypted volume start sector
	 * Consists of 4 bytes
	 */
	uint8_t encrypted_volume_start_sector[ 4 ];

	/* The master key size
	 * Consists of 4 bytes
	 */
	uint8_t master_key_size[ 4 ];

	/* The master key validation hash
	 * Consists of 20 bytes
	 */
	uint8_t master_key_validation_hash[ 20 ];

	/* The master key salt
	 * Consists of 32 bytes
	 */
	uint8_t master_key_salt[ 32 ];

	/* The master key number of iterations
	 * Consists of 4 bytes
	 */
	uint8_t master_key_number_of_iterations[ 4 ];

	/* The volume identifier
	 * Consists of 40 bytes
	 * Contains: an ASCII string with an end-of-string character
	 */
	uint8_t volume_identifier[ 40 ];
};

typedef struct luksde_volume_header_v2 luksde_volume_header_v2_t;

struct luksde_volume_header_v2
{
	/* The signature
	 * Consists of 6 bytes
	 * Contains: LUKS\xba\xbe
	 */
	uint8_t signature[ 6 ];

	/* The format version
	 * Consists of 2 bytes
	 * Contains: 2
	 */
	uint8_t format_version[ 2 ];

	/* The metadata area size
	 * Consists of 8 bytes
	 */
	uint8_t metadata_area_size[ 8 ];

	/* The sequence identifier
	 * Consists of 8 bytes
	 */
	uint8_t sequence_identifier[ 8 ];

	/* The label
	 * Consists of 48 bytes
	 * Contains: an ASCII string with an end-of-string character
	 */
	uint8_t label[ 48 ];

	/* The checksum method
	 * Consists of 32 bytes
	 * Contains: an ASCII string with an end-of-string character
	 */
	uint8_t checksum_method[ 32 ];

	/* The salt
	 * Consists of 64 bytes
	 */
	uint8_t salt[ 64 ];

	/* The volume identifier
	 * Consists of 40 bytes
	 * Contains: an ASCII string with an end-of-string character
	 */
	uint8_t volume_identifier[ 40 ];

	/* The subsystem
	 * Consists of 48 bytes
	 * Contains: an ASCII string with an end-of-string character
	 */
	uint8_t subsystem[ 48 ];

	/* The header offset
	 * Consists of 8 bytes
	 */
	uint8_t header_offset[ 8 ];

	/* Unknown (padding)
	 * Consists of 184 bytes
	 */
	uint8_t padding1[ 184 ];

	/* The checksum
	 * Consists of 64 bytes
	 */
	uint8_t checksum[ 64 ];

	/* Unknown (padding)
	 * Consists of 3584 bytes
	 */
	uint8_t padding2[ 3584 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LUKSDE_VOLUME_H ) */
