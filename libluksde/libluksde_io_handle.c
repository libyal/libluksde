/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2013-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libluksde_codepage.h"
#include "libluksde_definitions.h"
#include "libluksde_encryption.h"
#include "libluksde_io_handle.h"
#include "libluksde_libbfio.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"
#include "libluksde_libfdata.h"
#include "libluksde_libuna.h"
#include "libluksde_sector_data.h"
#include "libluksde_unused.h"
#include "libluksde_volume_header.h"

const uint8_t luksde_signature[ 6 ] = { 'L', 'U', 'K', 'S', 0xba, 0xbe };

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_initialize(
     libluksde_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libluksde_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libluksde_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libluksde_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	( *io_handle )->bytes_per_sector = 512;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_free(
     libluksde_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libluksde_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( libluksde_io_handle_clear(
		     *io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_clear(
     libluksde_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libluksde_io_handle_clear";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->encryption_context != NULL )
	{
		if( libluksde_encryption_free(
		     &( io_handle->encryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encryption context.",
			 function );

			result = -1;
		}
	}
	io_handle->bytes_per_sector = 512;

	return( result );
}

/* Reads a sector
 * Callback function for the volume vector
 * Returns 1 if successful or -1 on error
 */
int libluksde_io_handle_read_sector(
     libluksde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index LIBLUKSDE_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size LIBLUKSDE_ATTRIBUTE_UNUSED,
     uint32_t element_data_flags LIBLUKSDE_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBLUKSDE_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libluksde_sector_data_t *sector_data = NULL;
	static char *function                = "libluksde_io_handle_read_sector";

	LIBLUKSDE_UNREFERENCED_PARAMETER( element_data_file_index );
	LIBLUKSDE_UNREFERENCED_PARAMETER( element_data_size );
	LIBLUKSDE_UNREFERENCED_PARAMETER( element_data_flags );
	LIBLUKSDE_UNREFERENCED_PARAMETER( read_flags );

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
/* TODO handle virtual sectors, what about different sector sizes? */
	if( libluksde_sector_data_initialize(
	     &sector_data,
	     (size_t) io_handle->bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( libluksde_sector_data_read(
	     sector_data,
	     io_handle,
	     file_io_handle,
	     element_data_offset,
	     io_handle->encryption_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sector data.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) sector_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libluksde_sector_data_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sector data as element value.",
		 function );

		return( -1 );
	}
	return( 1 );

on_error:
	if( sector_data != NULL )
	{
		libluksde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( -1 );
}

