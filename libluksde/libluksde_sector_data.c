/*
 * Sector data functions
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
#include "libluksde_encryption_context.h"
#include "libluksde_libbfio.h"
#include "libluksde_libcerror.h"
#include "libluksde_libcnotify.h"
#include "libluksde_sector_data.h"

/* Creates sector data
 * Make sure the value sector_data is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libluksde_sector_data_initialize(
     libluksde_sector_data_t **sector_data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libluksde_sector_data_initialize";

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( *sector_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sector data value already set.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	*sector_data = memory_allocate_structure(
	                libluksde_sector_data_t );

	if( *sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sector_data,
	     0,
	     sizeof( libluksde_sector_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector data.",
		 function );

		memory_free(
		 *sector_data );

		*sector_data = NULL;

		return( -1 );
	}
	( *sector_data )->data = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * data_size );

	if( ( *sector_data )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	( *sector_data )->data_size = data_size;

	return( 1 );

on_error:
	if( *sector_data != NULL )
	{
		memory_free(
		 *sector_data );

		*sector_data = NULL;
	}
	return( -1 );
}

/* Frees sector data
 * Returns 1 if successful or -1 on error
 */
int libluksde_sector_data_free(
     libluksde_sector_data_t **sector_data,
     libcerror_error_t **error )
{
	static char *function = "libluksde_sector_data_free";
	int result            = 1;

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( *sector_data != NULL )
	{
		if( memory_set(
		     ( *sector_data )->data,
		     0,
		     ( *sector_data )->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.",
			 function );

			result = -1;
		}
		memory_free(
		 ( *sector_data )->data );

		memory_free(
		 *sector_data );

		*sector_data = NULL;
	}
	return( result );
}

/* Reads sector data
 * Returns 1 if successful or -1 on error
 */
int libluksde_sector_data_read_file_io_handle(
     libluksde_sector_data_t *sector_data,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libluksde_encryption_context_t *encryption_context,
     uint64_t sector_number,
     libcerror_error_t **error )
{
	uint8_t *encrypted_data = NULL;
	static char *function   = "libluksde_sector_data_read_file_io_handle";
	ssize_t read_count      = 0;

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( sector_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sector data - missing data.",
		 function );

		return( -1 );
	}
	encrypted_data = (uint8_t *) memory_allocate(
	                              sizeof( uint8_t ) * sector_data->data_size );

	if( encrypted_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create encrypted data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading sector: %" PRIu64 " data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 sector_number,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              encrypted_data,
	              sector_data->data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) sector_data->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sector: %" PRIu64 " data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 sector_number,
		 file_offset,
		 file_offset );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: encrypted sector: %" PRIu64 " data:\n",
		 function,
		 sector_number );
		libcnotify_print_data(
		 encrypted_data,
		 sector_data->data_size,
		 0 );
	}
#endif
	if( libluksde_encryption_context_crypt(
	     encryption_context,
	     LIBLUKSDE_ENCRYPTION_CRYPT_MODE_DECRYPT,
	     encrypted_data,
	     sector_data->data_size,
	     sector_data->data,
	     sector_data->data_size,
	     sector_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
		 "%s: unable to decrypt sector: %" PRIu64 " data.",
		 function,
		 sector_number );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: sector: %" PRIu64 " data:\n",
		 function,
		 sector_number );
		libcnotify_print_data(
		 sector_data->data,
		 sector_data->data_size,
		 0 );
	}
#endif
	memory_free(
	 encrypted_data );

	return( 1 );

on_error:
	if( encrypted_data != NULL )
	{
		memory_free(
		 encrypted_data );
	}
	return( -1 );
}

