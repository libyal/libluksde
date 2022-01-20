/*
 * Library volume type test program
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
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "luksde_test_functions.h"
#include "luksde_test_getopt.h"
#include "luksde_test_libbfio.h"
#include "luksde_test_libcerror.h"
#include "luksde_test_libluksde.h"
#include "luksde_test_macros.h"
#include "luksde_test_memory.h"

#include "../libluksde/libluksde_volume.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make luksde_test_volume generate verbose output
#define LUKSDE_TEST_VOLUME_VERBOSE
 */

#define LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE	4096

#if !defined( LIBLUKSDE_HAVE_BFIO )

LIBLUKSDE_EXTERN \
int libluksde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_volume_open_file_io_handle(
     libluksde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libluksde_error_t **error );

#endif /* !defined( LIBLUKSDE_HAVE_BFIO ) */

/* Creates and opens a source volume
 * Returns 1 if successful or -1 on error
 */
int luksde_test_volume_open_source(
     libluksde_volume_t **volume,
     libbfio_handle_t *file_io_handle,
     const system_character_t *password,
     libcerror_error_t **error )
{
	static char *function = "luksde_test_volume_open_source";
	size_t string_length  = 0;
	int result            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libluksde_volume_initialize(
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize volume.",
		 function );

		goto on_error;
	}
	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libluksde_volume_set_utf16_password(
		          *volume,
		          (uint16_t *) password,
		          string_length,
		          error );
#else
		result = libluksde_volume_set_utf8_password(
		          *volume,
		          (uint8_t *) password,
		          string_length,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set password.",
			 function );

			goto on_error;
		}
	}
	result = libluksde_volume_open_file_io_handle(
	          *volume,
	          file_io_handle,
	          LIBLUKSDE_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume != NULL )
	{
		libluksde_volume_free(
		 volume,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source volume
 * Returns 1 if successful or -1 on error
 */
int luksde_test_volume_close_source(
     libluksde_volume_t **volume,
     libcerror_error_t **error )
{
	static char *function = "luksde_test_volume_close_source";
	int result            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( libluksde_volume_close(
	     *volume,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close volume.",
		 function );

		result = -1;
	}
	if( libluksde_volume_free(
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libluksde_volume_initialize function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libluksde_volume_t *volume      = NULL;
	int result                      = 0;

#if defined( HAVE_LUKSDE_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libluksde_volume_initialize(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_volume_free(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_initialize(
	          NULL,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	volume = (libluksde_volume_t *) 0x12345678UL;

	result = libluksde_volume_initialize(
	          &volume,
	          &error );

	volume = NULL;

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_LUKSDE_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libluksde_volume_initialize with malloc failing
		 */
		luksde_test_malloc_attempts_before_fail = test_number;

		result = libluksde_volume_initialize(
		          &volume,
		          &error );

		if( luksde_test_malloc_attempts_before_fail != -1 )
		{
			luksde_test_malloc_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libluksde_volume_free(
				 &volume,
				 NULL );
			}
		}
		else
		{
			LUKSDE_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			LUKSDE_TEST_ASSERT_IS_NULL(
			 "volume",
			 volume );

			LUKSDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libluksde_volume_initialize with memset failing
		 */
		luksde_test_memset_attempts_before_fail = test_number;

		result = libluksde_volume_initialize(
		          &volume,
		          &error );

		if( luksde_test_memset_attempts_before_fail != -1 )
		{
			luksde_test_memset_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libluksde_volume_free(
				 &volume,
				 NULL );
			}
		}
		else
		{
			LUKSDE_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			LUKSDE_TEST_ASSERT_IS_NULL(
			 "volume",
			 volume );

			LUKSDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_LUKSDE_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libluksde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_volume_free function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libluksde_volume_free(
	          NULL,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libluksde_volume_open function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_open(
     const system_character_t *source,
     const system_character_t *password )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error   = NULL;
	libluksde_volume_t *volume = NULL;
	size_t string_length       = 0;
	int result                 = 0;

	/* Initialize test
	 */
	result = luksde_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_volume_initialize(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libluksde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libluksde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        LUKSDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libluksde_volume_open(
	          volume,
	          narrow_source,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_open(
	          NULL,
	          narrow_source,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_volume_open(
	          volume,
	          NULL,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_volume_open(
	          volume,
	          narrow_source,
	          -1,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test open when already opened
	 */
	result = libluksde_volume_open(
	          volume,
	          narrow_source,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libluksde_volume_free(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libluksde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libluksde_volume_open_wide function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_open_wide(
     const system_character_t *source,
     const system_character_t *password )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error   = NULL;
	libluksde_volume_t *volume = NULL;
	size_t string_length       = 0;
	int result                 = 0;

	/* Initialize test
	 */
	result = luksde_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_volume_initialize(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libluksde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libluksde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        LUKSDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libluksde_volume_open_wide(
	          volume,
	          wide_source,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_open_wide(
	          NULL,
	          wide_source,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_volume_open_wide(
	          volume,
	          NULL,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_volume_open_wide(
	          volume,
	          wide_source,
	          -1,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test open when already opened
	 */
	result = libluksde_volume_open_wide(
	          volume,
	          wide_source,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libluksde_volume_free(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libluksde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libluksde_volume_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_open_file_io_handle(
     const system_character_t *source,
     const system_character_t *password )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libluksde_volume_t *volume       = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_volume_initialize(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libluksde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libluksde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        LUKSDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libluksde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_volume_open_file_io_handle(
	          volume,
	          NULL,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          -1,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test open when already opened
	 */
	result = libluksde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          LIBLUKSDE_OPEN_READ,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libluksde_volume_free(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "file_io_handle",
	 file_io_handle );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libluksde_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_volume_close function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libluksde_volume_close(
	          NULL,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libluksde_volume_open and libluksde_volume_close functions
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_open_close(
     const system_character_t *source,
     const system_character_t *password )
{
	libcerror_error_t *error   = NULL;
	libluksde_volume_t *volume = NULL;
	size_t string_length       = 0;
	int result                 = 0;

	/* Initialize test
	 */
	result = libluksde_volume_initialize(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libluksde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libluksde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        LUKSDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libluksde_volume_open_wide(
	          volume,
	          source,
	          LIBLUKSDE_OPEN_READ,
	          &error );
#else
	result = libluksde_volume_open(
	          volume,
	          source,
	          LIBLUKSDE_OPEN_READ,
	          &error );
#endif

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_volume_close(
	          volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libluksde_volume_open_wide(
	          volume,
	          source,
	          LIBLUKSDE_OPEN_READ,
	          &error );
#else
	result = libluksde_volume_open(
	          volume,
	          source,
	          LIBLUKSDE_OPEN_READ,
	          &error );
#endif

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_volume_close(
	          volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libluksde_volume_free(
	          &volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libluksde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_volume_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_signal_abort(
     libluksde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libluksde_volume_signal_abort(
	          volume,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_signal_abort(
	          NULL,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libluksde_volume_is_locked function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_is_locked(
     libluksde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libluksde_volume_is_locked(
	          volume,
	          &error );

	LUKSDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_is_locked(
	          NULL,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

/* Tests the libluksde_internal_volume_read_buffer_from_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_internal_volume_read_buffer_from_file_io_handle(
     libluksde_volume_t *volume )
{
	uint8_t buffer[ LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libluksde_volume_get_size(
	          volume,
	          &size,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE;

	if( size < LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libluksde_internal_volume_read_buffer_from_file_io_handle(
	              (libluksde_internal_volume_t *) volume,
	              ( (libluksde_internal_volume_t *) volume )->file_io_handle,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Set offset to size - 8
		 */
		offset = libluksde_volume_seek_offset(
		          volume,
		          -8,
		          SEEK_END,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) size - 8 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on size boundary
		 */
		read_count = libluksde_internal_volume_read_buffer_from_file_io_handle(
		              (libluksde_internal_volume_t *) volume,
		              ( (libluksde_internal_volume_t *) volume )->file_io_handle,
		              buffer,
		              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              &error );

		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libluksde_internal_volume_read_buffer_from_file_io_handle(
		              (libluksde_internal_volume_t *) volume,
		              ( (libluksde_internal_volume_t *) volume )->file_io_handle,
		              buffer,
		              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              &error );

		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	remaining_size = size;

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		LUKSDE_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		read_size = (size_t) random_number % LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE;

#if defined( LUKSDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libluksde_volume_read_buffer: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libluksde_internal_volume_read_buffer_from_file_io_handle(
		              (libluksde_internal_volume_t *) volume,
		              ( (libluksde_internal_volume_t *) volume )->file_io_handle,
		              buffer,
		              read_size,
		              &error );

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libluksde_volume_get_offset(
		          volume,
		          &offset,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		LUKSDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		remaining_size -= read_count;

		if( remaining_size == 0 )
		{
			offset = libluksde_volume_seek_offset(
			          volume,
			          0,
			          SEEK_SET,
			          &error );

			LUKSDE_TEST_ASSERT_EQUAL_INT64(
			 "offset",
			 offset,
			 (int64_t) 0 );

			LUKSDE_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			read_offset = 0;

			remaining_size = size;
		}
	}
	/* Reset offset to 0
	 */
	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libluksde_internal_volume_read_buffer_from_file_io_handle(
	              NULL,
	              ( (libluksde_internal_volume_t *) volume )->file_io_handle,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libluksde_internal_volume_read_buffer_from_file_io_handle(
	              (libluksde_internal_volume_t *) volume,
	              ( (libluksde_internal_volume_t *) volume )->file_io_handle,
	              NULL,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libluksde_internal_volume_read_buffer_from_file_io_handle(
	              (libluksde_internal_volume_t *) volume,
	              ( (libluksde_internal_volume_t *) volume )->file_io_handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

/* Tests the libluksde_volume_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_read_buffer(
     libluksde_volume_t *volume )
{
	uint8_t buffer[ LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libluksde_volume_get_size(
	          volume,
	          &size,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE;

	if( size < LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libluksde_volume_read_buffer(
	              volume,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Set offset to size - 8
		 */
		offset = libluksde_volume_seek_offset(
		          volume,
		          -8,
		          SEEK_END,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) size - 8 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on size boundary
		 */
		read_count = libluksde_volume_read_buffer(
		              volume,
		              buffer,
		              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              &error );

		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libluksde_volume_read_buffer(
		              volume,
		              buffer,
		              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              &error );

		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	remaining_size = size;

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		LUKSDE_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		read_size = (size_t) random_number % LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE;

#if defined( LUKSDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libluksde_volume_read_buffer: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libluksde_volume_read_buffer(
		              volume,
		              buffer,
		              read_size,
		              &error );

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libluksde_volume_get_offset(
		          volume,
		          &offset,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		LUKSDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		remaining_size -= read_count;

		if( remaining_size == 0 )
		{
			offset = libluksde_volume_seek_offset(
			          volume,
			          0,
			          SEEK_SET,
			          &error );

			LUKSDE_TEST_ASSERT_EQUAL_INT64(
			 "offset",
			 offset,
			 (int64_t) 0 );

			LUKSDE_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			read_offset = 0;

			remaining_size = size;
		}
	}
	/* Reset offset to 0
	 */
	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libluksde_volume_read_buffer(
	              NULL,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libluksde_volume_read_buffer(
	              volume,
	              NULL,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libluksde_volume_read_buffer(
	              volume,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_LUKSDE_TEST_RWLOCK )

	/* Test libluksde_volume_read_buffer with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	luksde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	read_count = libluksde_volume_read_buffer(
	              volume,
	              buffer,
	              LUKSDE_TEST_PARTITION_READ_BUFFER_SIZE,
	              &error );

	if( luksde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		luksde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		LUKSDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libluksde_volume_read_buffer with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	luksde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	read_count = libluksde_volume_read_buffer(
	              volume,
	              buffer,
	              LUKSDE_TEST_PARTITION_READ_BUFFER_SIZE,
	              &error );

	if( luksde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		luksde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		LUKSDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_LUKSDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libluksde_volume_read_buffer_at_offset function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_read_buffer_at_offset(
     libluksde_volume_t *volume )
{
	uint8_t buffer[ LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libluksde_volume_get_size(
	          volume,
	          &size,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE;

	if( size < LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libluksde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Read buffer on size boundary
		 */
		read_count = libluksde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              size - 8,
		              &error );

		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libluksde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              size + 8,
		              &error );

		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		LUKSDE_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		if( size > 0 )
		{
			read_offset = (off64_t) random_number % size;
		}
		read_size = (size_t) random_number % LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE;

#if defined( LUKSDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libluksde_volume_read_buffer_at_offset: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libluksde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              read_size,
		              read_offset,
		              &error );

		remaining_size = size - read_offset;

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libluksde_volume_get_offset(
		          volume,
		          &offset,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		LUKSDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	read_count = libluksde_volume_read_buffer_at_offset(
	              NULL,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libluksde_volume_read_buffer_at_offset(
	              volume,
	              NULL,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libluksde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libluksde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              -1,
	              &error );

	LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_LUKSDE_TEST_RWLOCK )

	/* Test libluksde_volume_read_buffer_at_offset with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	luksde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	read_count = libluksde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	if( luksde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		luksde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		LUKSDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libluksde_volume_read_buffer_at_offset with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	luksde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	read_count = libluksde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              LUKSDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	if( luksde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		luksde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		LUKSDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		LUKSDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_LUKSDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libluksde_volume_seek_offset function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_seek_offset(
     libluksde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	off64_t offset           = 0;

	/* Test regular cases
	 */
	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_END,
	          &error );

	LUKSDE_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size = (size64_t) offset;

	offset = libluksde_volume_seek_offset(
	          volume,
	          1024,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 1024 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libluksde_volume_seek_offset(
	          volume,
	          -512,
	          SEEK_CUR,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 512 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libluksde_volume_seek_offset(
	          volume,
	          (off64_t) ( size + 512 ),
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) ( size + 512 ) );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libluksde_volume_seek_offset(
	          NULL,
	          0,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libluksde_volume_seek_offset(
	          volume,
	          -1,
	          SEEK_SET,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libluksde_volume_seek_offset(
	          volume,
	          -1,
	          SEEK_CUR,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	offset = libluksde_volume_seek_offset(
	          volume,
	          (off64_t) ( -1 * ( size + 1 ) ),
	          SEEK_END,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_LUKSDE_TEST_RWLOCK )

	/* Test libluksde_volume_seek_offset with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	luksde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	if( luksde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		luksde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		LUKSDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 (int64_t) offset,
		 (int64_t) -1 );

		LUKSDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libluksde_volume_seek_offset with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	luksde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	offset = libluksde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	if( luksde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		luksde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		LUKSDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 (int64_t) offset,
		 (int64_t) -1 );

		LUKSDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_LUKSDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libluksde_volume_get_offset function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_get_offset(
     libluksde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libluksde_volume_get_offset(
	          volume,
	          &offset,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_get_offset(
	          NULL,
	          &offset,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_volume_get_offset(
	          volume,
	          NULL,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libluksde_volume_get_size function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_get_size(
     libluksde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libluksde_volume_get_size(
	          volume,
	          &size,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_get_size(
	          NULL,
	          &size,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_volume_get_size(
	          volume,
	          NULL,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle    = NULL;
	libcerror_error_t *error            = NULL;
	libluksde_volume_t *volume          = NULL;
	system_character_t *option_password = NULL;
	system_character_t *source          = NULL;
	system_integer_t option             = 0;
	size_t string_length                = 0;
	int result                          = 0;

	while( ( option = luksde_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "p:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (system_integer_t) 'p':
				option_password = optarg;

				break;
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( LUKSDE_TEST_VOLUME_VERBOSE )
	libluksde_notify_set_verbose(
	 1 );
	libluksde_notify_set_stream(
	 stderr,
	 NULL );
#endif

	LUKSDE_TEST_RUN(
	 "libluksde_volume_initialize",
	 luksde_test_volume_initialize );

	LUKSDE_TEST_RUN(
	 "libluksde_volume_free",
	 luksde_test_volume_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_initialize(
		          &file_io_handle,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	         "file_io_handle",
	         file_io_handle );

	        LUKSDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        LUKSDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libluksde_check_volume_signature_file_io_handle(
		          file_io_handle,
		          &error );

		LUKSDE_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( result != 0 )
	{
		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_open",
		 luksde_test_volume_open,
		 source,
		 option_password );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_open_wide",
		 luksde_test_volume_open_wide,
		 source,
		 option_password );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_open_file_io_handle",
		 luksde_test_volume_open_file_io_handle,
		 source,
		 option_password );

		LUKSDE_TEST_RUN(
		 "libluksde_volume_close",
		 luksde_test_volume_close );

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_open_close",
		 luksde_test_volume_open_close,
		 source,
		 option_password );

		/* Initialize volume for tests
		 */
		result = luksde_test_volume_open_source(
		          &volume,
		          file_io_handle,
		          option_password,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		LUKSDE_TEST_ASSERT_IS_NOT_NULL(
		 "volume",
		 volume );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_signal_abort",
		 luksde_test_volume_signal_abort,
		 volume );

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

		/* TODO: add tests for libluksde_volume_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_is_locked",
		 luksde_test_volume_is_locked,
		 volume );

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_internal_volume_read_buffer_from_file_io_handle",
		 luksde_test_internal_volume_read_buffer_from_file_io_handle,
		 volume );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_read_buffer",
		 luksde_test_volume_read_buffer,
		 volume );

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_read_buffer_at_offset",
		 luksde_test_volume_read_buffer_at_offset,
		 volume );

		/* TODO: add tests for libluksde_volume_write_buffer */

		/* TODO: add tests for libluksde_volume_write_buffer_at_offset */

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

		/* TODO: add tests for libluksde_internal_volume_seek_offset */

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_seek_offset",
		 luksde_test_volume_seek_offset,
		 volume );

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_get_offset",
		 luksde_test_volume_get_offset,
		 volume );

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_get_size",
		 luksde_test_volume_get_size,
		 volume );

		/* TODO: add tests for libluksde_volume_get_encryption_method */

		/* TODO: add tests for libluksde_volume_get_volume_identifier */

		/* TODO: add tests for libluksde_volume_set_key */

		/* TODO: add tests for libluksde_volume_set_keys */

		/* TODO: add tests for libluksde_volume_set_utf8_password */

		/* TODO: add tests for libluksde_volume_set_utf16_password */

		/* Clean up
		 */
		result = luksde_test_volume_close_source(
		          &volume,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "volume",
		 volume );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( file_io_handle != NULL )
	{
		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		LUKSDE_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        LUKSDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libluksde_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

