/*
 * Library sector_data_vector type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "luksde_test_functions.h"
#include "luksde_test_libluksde.h"
#include "luksde_test_libbfio.h"
#include "luksde_test_libcerror.h"
#include "luksde_test_macros.h"
#include "luksde_test_memory.h"
#include "luksde_test_unused.h"

#include "../libluksde/libluksde_definitions.h"
#include "../libluksde/libluksde_sector_data_vector.h"

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

/* Tests the libluksde_sector_data_vector_initialize function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_sector_data_vector_initialize(
     void )
{
	libcerror_error_t *error                           = NULL;
	libluksde_sector_data_vector_t *sector_data_vector = NULL;
	int result                                         = 0;

#if defined( HAVE_LUKSDE_TEST_MEMORY )
	int number_of_malloc_fail_tests                    = 1;
	int number_of_memset_fail_tests                    = 1;
	int test_number                                    = 0;
#endif

	/* Test regular cases
	 */
	result = libluksde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          0,
	          4096,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_sector_data_vector_free(
	          &sector_data_vector,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_sector_data_vector_initialize(
	          NULL,
	          512,
	          0,
	          4096,
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

	sector_data_vector = (libluksde_sector_data_vector_t *) 0x12345678UL;

	result = libluksde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          0,
	          4096,
	          &error );

	sector_data_vector = NULL;

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
		/* Test libluksde_sector_data_vector_initialize with malloc failing
		 */
		luksde_test_malloc_attempts_before_fail = test_number;

		result = libluksde_sector_data_vector_initialize(
		          &sector_data_vector,
		          512,
		          0,
		          4096,
		          &error );

		if( luksde_test_malloc_attempts_before_fail != -1 )
		{
			luksde_test_malloc_attempts_before_fail = -1;

			if( sector_data_vector != NULL )
			{
				libluksde_sector_data_vector_free(
				 &sector_data_vector,
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
			 "sector_data_vector",
			 sector_data_vector );

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
		/* Test libluksde_sector_data_vector_initialize with memset failing
		 */
		luksde_test_memset_attempts_before_fail = test_number;

		result = libluksde_sector_data_vector_initialize(
		          &sector_data_vector,
		          512,
		          0,
		          4096,
		          &error );

		if( luksde_test_memset_attempts_before_fail != -1 )
		{
			luksde_test_memset_attempts_before_fail = -1;

			if( sector_data_vector != NULL )
			{
				libluksde_sector_data_vector_free(
				 &sector_data_vector,
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
			 "sector_data_vector",
			 sector_data_vector );

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
	if( sector_data_vector != NULL )
	{
		libluksde_sector_data_vector_free(
		 &sector_data_vector,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_sector_data_vector_free function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_sector_data_vector_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libluksde_sector_data_vector_free(
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

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc LUKSDE_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] LUKSDE_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc LUKSDE_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] LUKSDE_TEST_ATTRIBUTE_UNUSED )
#endif
{
	LUKSDE_TEST_UNREFERENCED_PARAMETER( argc )
	LUKSDE_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

	LUKSDE_TEST_RUN(
	 "libluksde_sector_data_vector_initialize",
	 luksde_test_sector_data_vector_initialize );

	LUKSDE_TEST_RUN(
	 "libluksde_sector_data_vector_free",
	 luksde_test_sector_data_vector_free );

	/* TODO add tests for libluksde_sector_data_vector_get_sector_data_at_offset */

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */
}

