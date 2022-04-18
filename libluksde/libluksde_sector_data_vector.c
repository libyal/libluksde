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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libluksde_definitions.h"
#include "libluksde_encryption_context.h"
#include "libluksde_libbfio.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"
#include "libluksde_libfcache.h"
#include "libluksde_sector_data.h"
#include "libluksde_sector_data_vector.h"

/* Creates a sector data vector
 * Make sure the value sector data vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libluksde_sector_data_vector_initialize(
     libluksde_sector_data_vector_t **sector_data_vector,
     uint16_t bytes_per_sector,
     off64_t data_offset,
     size64_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libluksde_sector_data_vector_initialize";

	if( sector_data_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data vector.",
		 function );

		return( -1 );
	}
	if( *sector_data_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sector data vector value already set.",
		 function );

		return( -1 );
	}
	if( data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid bytes per sector value out of bounds.",
		 function );

		return( -1 );
	}
	*sector_data_vector = memory_allocate_structure(
	                       libluksde_sector_data_vector_t );

	if( *sector_data_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector data vector.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sector_data_vector,
	     0,
	     sizeof( libluksde_sector_data_vector_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector data vector.",
		 function );

		goto on_error;
	}
	if( libfcache_date_time_get_timestamp(
	     &( ( *sector_data_vector )->cache_timestamp ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cache timestamp.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *sector_data_vector )->cache ),
	     LIBLUKSDE_MAXIMUM_CACHE_ENTRIES_SECTOR_DATA,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sectors cache.",
		 function );

		goto on_error;
	}
	( *sector_data_vector )->bytes_per_sector = bytes_per_sector;
	( *sector_data_vector )->data_offset      = data_offset;
	( *sector_data_vector )->data_size        = data_size;

	return( 1 );

on_error:
	if( *sector_data_vector != NULL )
	{
		memory_free(
		 *sector_data_vector );

		*sector_data_vector = NULL;
	}
	return( -1 );
}

/* Frees a sector data vector
 * Returns 1 if successful or -1 on error
 */
int libluksde_sector_data_vector_free(
     libluksde_sector_data_vector_t **sector_data_vector,
     libcerror_error_t **error )
{
	static char *function = "libluksde_sector_data_vector_free";
	int result            = 1;

	if( sector_data_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data vector.",
		 function );

		return( -1 );
	}
	if( *sector_data_vector != NULL )
	{
		if( libfcache_cache_free(
		     &( ( *sector_data_vector )->cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *sector_data_vector );

		*sector_data_vector = NULL;
	}
	return( result );
}

/* Reads sector data at a specific offset
 * Returns 1 if successful or -1 on error
 */
int libluksde_sector_data_vector_get_sector_data_at_offset(
     libluksde_sector_data_vector_t *sector_data_vector,
     libbfio_handle_t *file_io_handle,
     libluksde_encryption_context_t *encryption_context,
     off64_t offset,
     libluksde_sector_data_t **sector_data,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value      = NULL;
	libluksde_sector_data_t *safe_sector_data = NULL;
	static char *function                     = "libluksde_sector_data_vector_get_sector_data_at_offset";
	off64_t sector_data_offset                = 0;
	uint64_t sector_number                    = 0;
	int result                                = 0;

	if( sector_data_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data vector.",
		 function );

		return( -1 );
	}
	result = libfcache_cache_get_value_by_identifier(
	          sector_data_vector->cache,
	          0,
	          offset,
	          sector_data_vector->cache_timestamp,
	          &cache_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from cache.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: cache: 0x%08" PRIjx " hit for offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 (intptr_t) sector_data_vector->cache,
			 offset,
			 offset );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfcache_cache_value_get_value(
		     cache_value,
		     (intptr_t **) sector_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sector data from cache.",
			 function );

			goto on_error;
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: cache: 0x%08" PRIjx " miss for offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 (intptr_t) sector_data_vector->cache,
			 offset,
			 offset );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libluksde_sector_data_initialize(
		     &safe_sector_data,
		     (size_t) sector_data_vector->bytes_per_sector,
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
		sector_data_offset = sector_data_vector->data_offset + offset;
		sector_number      = (uint64_t) offset / sector_data_vector->bytes_per_sector;

		if( libluksde_sector_data_read_file_io_handle(
		     safe_sector_data,
		     file_io_handle,
		     sector_data_offset,
		     encryption_context,
		     sector_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sector: %" PRIu64 " data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 sector_number,
			 sector_data_offset,
			 sector_data_offset );

			goto on_error;
		}
		if( libfcache_cache_set_value_by_identifier(
		     sector_data_vector->cache,
		     0,
		     offset,
		     sector_data_vector->cache_timestamp,
		     (intptr_t *) safe_sector_data,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libluksde_sector_data_free,
		     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set sector data in cache.",
			 function );

			goto on_error;
		}
		*sector_data     = safe_sector_data;
		safe_sector_data = NULL;
	}
	return( 1 );

on_error:
	if( safe_sector_data != NULL )
	{
		libluksde_sector_data_free(
		 &safe_sector_data,
		 NULL );
	}
	return( -1 );
}

