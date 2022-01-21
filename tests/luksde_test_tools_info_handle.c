/*
 * Tools info_handle type test program
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "luksde_test_libcerror.h"
#include "luksde_test_macros.h"
#include "luksde_test_memory.h"
#include "luksde_test_unused.h"

#include "../luksdetools/info_handle.h"

/* Tests the info_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_tools_info_handle_initialize(
     void )
{
	info_handle_t *info_handle      = NULL;
	libcerror_error_t *error        = NULL;
	int result                      = 0;

#if defined( HAVE_LUKSDE_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = info_handle_initialize(
	          &info_handle,
	          1,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "info_handle",
	 info_handle );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = info_handle_free(
	          &info_handle,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "info_handle",
	 info_handle );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = info_handle_initialize(
	          NULL,
	          1,
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

	info_handle = (info_handle_t *) 0x12345678UL;

	result = info_handle_initialize(
	          &info_handle,
	          1,
	          &error );

	info_handle = NULL;

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
		/* Test info_handle_initialize with malloc failing
		 */
		luksde_test_malloc_attempts_before_fail = test_number;

		result = info_handle_initialize(
		          &info_handle,
		          1,
		          &error );

		if( luksde_test_malloc_attempts_before_fail != -1 )
		{
			luksde_test_malloc_attempts_before_fail = -1;

			if( info_handle != NULL )
			{
				info_handle_free(
				 &info_handle,
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
			 "info_handle",
			 info_handle );

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
		/* Test info_handle_initialize with memset failing
		 */
		luksde_test_memset_attempts_before_fail = test_number;

		result = info_handle_initialize(
		          &info_handle,
		          1,
		          &error );

		if( luksde_test_memset_attempts_before_fail != -1 )
		{
			luksde_test_memset_attempts_before_fail = -1;

			if( info_handle != NULL )
			{
				info_handle_free(
				 &info_handle,
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
			 "info_handle",
			 info_handle );

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
	if( info_handle != NULL )
	{
		info_handle_free(
		 &info_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the info_handle_free function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_tools_info_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = info_handle_free(
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

	LUKSDE_TEST_RUN(
	 "info_handle_initialize",
	 luksde_test_tools_info_handle_initialize );

	LUKSDE_TEST_RUN(
	 "info_handle_free",
	 luksde_test_tools_info_handle_free );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

