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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libluksde_definitions.h"
#include "libluksde_key_slot.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"

#include "luksde_keyslot.h"

/* Creates a key slot
 * Make sure the value key_slot is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libluksde_key_slot_initialize(
     libluksde_key_slot_t **key_slot,
     libcerror_error_t **error )
{
	static char *function = "libluksde_key_slot_initialize";

	if( key_slot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key slot.",
		 function );

		return( -1 );
	}
	if( *key_slot != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key slot value already set.",
		 function );

		return( -1 );
	}
	*key_slot = memory_allocate_structure(
	             libluksde_key_slot_t );

	if( *key_slot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_slot,
	     0,
	     sizeof( libluksde_key_slot_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_slot != NULL )
	{
		memory_free(
		 *key_slot );

		*key_slot = NULL;
	}
	return( -1 );
}

/* Frees a key slot
 * Returns 1 if successful or -1 on error
 */
int libluksde_key_slot_free(
     libluksde_key_slot_t **key_slot,
     libcerror_error_t **error )
{
	static char *function = "libluksde_key_slot_free";

	if( key_slot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key slot.",
		 function );

		return( -1 );
	}
	if( *key_slot != NULL )
	{
		memory_free(
		 *key_slot );

		*key_slot = NULL;
	}
	return( 1 );
}

/* Reads a key slot
 * Returns 1 if successful or -1 on error
 */
int libluksde_key_slot_read_data(
     libluksde_key_slot_t *key_slot,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libluksde_key_slot_read_data";

	if( key_slot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key slot.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( luksde_volume_keyslot_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: keyslot data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( luksde_volume_keyslot_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_keyslot_t *) data )->state,
	 key_slot->state );

	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_keyslot_t *) data )->number_of_iterations,
	 key_slot->number_of_iterations );

	if( memory_copy(
	     key_slot->salt,
	     ( (luksde_volume_keyslot_t *) data )->salt,
	     32 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy salt to key slot.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_keyslot_t *) data )->start_sector,
	 key_slot->key_material_offset );

	byte_stream_copy_to_uint32_big_endian(
	 ( (luksde_volume_keyslot_t *) data )->number_of_stripes,
	 key_slot->number_of_stripes );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: state\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 key_slot->state );

		libcnotify_printf(
		 "%s: number of iterations\t\t\t: %" PRIu32 "\n",
		 function,
		 key_slot->number_of_iterations );

		libcnotify_printf(
		 "%s: salt:\n",
		 function );
		libcnotify_print_data(
		 ( (luksde_volume_keyslot_t *) data )->salt,
		 32,
		 0 );

		libcnotify_printf(
		 "%s: start sector\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 key_slot->key_material_offset );

		libcnotify_printf(
		 "%s: number of stripes\t\t\t\t: %" PRIu32 "\n",
		 function,
		 key_slot->number_of_stripes );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

