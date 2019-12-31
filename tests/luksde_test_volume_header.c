/*
 * Library volume_header type test program
 *
 * Copyright (C) 2013-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include "luksde_test_libbfio.h"
#include "luksde_test_libcerror.h"
#include "luksde_test_libluksde.h"
#include "luksde_test_macros.h"
#include "luksde_test_memory.h"
#include "luksde_test_unused.h"

#include "../libluksde/libluksde_volume_header.h"

uint8_t luksde_test_volume_header_data1[ 1024 ] = {
	0x4c, 0x55, 0x4b, 0x53, 0xba, 0xbe, 0x00, 0x01, 0x61, 0x65, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x62, 0x63, 0x2d, 0x65, 0x73, 0x73, 0x69,
	0x76, 0x3a, 0x73, 0x68, 0x61, 0x32, 0x35, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x68, 0x61, 0x31, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x20,
	0x3e, 0xb5, 0xc3, 0xed, 0x01, 0x29, 0x21, 0x67, 0x3c, 0x03, 0x61, 0x87, 0xa0, 0x7d, 0x39, 0x6e,
	0xc3, 0xc8, 0x4d, 0xa4, 0x25, 0xef, 0xf3, 0xfa, 0x9c, 0x7d, 0xe4, 0xff, 0x6b, 0x30, 0x51, 0x98,
	0x8f, 0x30, 0x3d, 0x70, 0x56, 0x73, 0xe7, 0xb3, 0x4d, 0xa1, 0x83, 0x9d, 0x63, 0xfa, 0x40, 0xbb,
	0x90, 0xd9, 0x7b, 0xb3, 0x00, 0x01, 0x02, 0xca, 0x32, 0x32, 0x38, 0x31, 0x34, 0x33, 0x63, 0x61,
	0x2d, 0x61, 0x37, 0x62, 0x32, 0x2d, 0x34, 0x30, 0x36, 0x32, 0x2d, 0x39, 0x38, 0x35, 0x65, 0x2d,
	0x32, 0x66, 0x37, 0x62, 0x61, 0x38, 0x39, 0x61, 0x32, 0x37, 0x63, 0x65, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xac, 0x71, 0xf3, 0x00, 0x04, 0x11, 0xaa, 0xdf, 0x3b, 0x5b, 0x16, 0x3f, 0x03, 0x3c, 0xc9,
	0xd7, 0x48, 0x18, 0x37, 0xf1, 0x7a, 0x36, 0x72, 0x1f, 0x3c, 0xc0, 0xda, 0x11, 0x81, 0xbb, 0x9a,
	0x6c, 0x7f, 0x91, 0xf9, 0x66, 0xdc, 0xb6, 0xef, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x0f, 0xa0,
	0x00, 0x00, 0xde, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x00, 0x0f, 0xa0,
	0x00, 0x00, 0xde, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x0f, 0xa0,
	0x00, 0x00, 0xde, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x0f, 0xa0,
	0x00, 0x00, 0xde, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x0f, 0xa0,
	0x00, 0x00, 0xde, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x08, 0x00, 0x00, 0x0f, 0xa0,
	0x00, 0x00, 0xde, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x08, 0x00, 0x00, 0x0f, 0xa0,
	0x00, 0x00, 0xde, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0x00, 0x00, 0x0f, 0xa0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

/* Tests the libluksde_volume_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_header_initialize(
     void )
{
	libcerror_error_t *error                 = NULL;
	libluksde_volume_header_t *volume_header = NULL;
	int result                               = 0;

#if defined( HAVE_LUKSDE_TEST_MEMORY )
	int number_of_malloc_fail_tests          = 1;
	int number_of_memset_fail_tests          = 1;
	int test_number                          = 0;
#endif

	/* Test regular cases
	 */
	result = libluksde_volume_header_initialize(
	          &volume_header,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume_header",
	 volume_header );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_volume_header_free(
	          &volume_header,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume_header",
	 volume_header );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_volume_header_initialize(
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

	volume_header = (libluksde_volume_header_t *) 0x12345678UL;

	result = libluksde_volume_header_initialize(
	          &volume_header,
	          &error );

	volume_header = NULL;

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
		/* Test libluksde_volume_header_initialize with malloc failing
		 */
		luksde_test_malloc_attempts_before_fail = test_number;

		result = libluksde_volume_header_initialize(
		          &volume_header,
		          &error );

		if( luksde_test_malloc_attempts_before_fail != -1 )
		{
			luksde_test_malloc_attempts_before_fail = -1;

			if( volume_header != NULL )
			{
				libluksde_volume_header_free(
				 &volume_header,
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
			 "volume_header",
			 volume_header );

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
		/* Test libluksde_volume_header_initialize with memset failing
		 */
		luksde_test_memset_attempts_before_fail = test_number;

		result = libluksde_volume_header_initialize(
		          &volume_header,
		          &error );

		if( luksde_test_memset_attempts_before_fail != -1 )
		{
			luksde_test_memset_attempts_before_fail = -1;

			if( volume_header != NULL )
			{
				libluksde_volume_header_free(
				 &volume_header,
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
			 "volume_header",
			 volume_header );

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
	if( volume_header != NULL )
	{
		libluksde_volume_header_free(
		 &volume_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_volume_header_free function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libluksde_volume_header_free(
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

/* Tests the libluksde_volume_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_header_read_data(
     void )
{
	libcerror_error_t *error                 = NULL;
	libluksde_volume_header_t *volume_header = NULL;
	int result                               = 0;

	/* Initialize test
	 */
	result = libluksde_volume_header_initialize(
	          &volume_header,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume_header",
	 volume_header );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libluksde_volume_header_read_data(
	          volume_header,
	          luksde_test_volume_header_data1,
	          1024,
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
	result = libluksde_volume_header_read_data(
	          NULL,
	          luksde_test_volume_header_data1,
	          1024,
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

	result = libluksde_volume_header_read_data(
	          volume_header,
	          NULL,
	          1024,
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

	result = libluksde_volume_header_read_data(
	          volume_header,
	          luksde_test_volume_header_data1,
	          (size_t) SSIZE_MAX + 1,
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

	result = libluksde_volume_header_read_data(
	          volume_header,
	          luksde_test_volume_header_data1,
	          0,
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

	/* Test error case where signature is invalid
	 */
	byte_stream_copy_from_uint64_big_endian(
	 luksde_test_volume_header_data1,
	 0xffffffffffff0001UL );

	result = libluksde_volume_header_read_data(
	          volume_header,
	          luksde_test_volume_header_data1,
	          1024,
	          &error );

	byte_stream_copy_from_uint64_big_endian(
	 luksde_test_volume_header_data1,
	 0x4c554b53babe0001UL );

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
	result = libluksde_volume_header_free(
	          &volume_header,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume_header",
	 volume_header );

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
	if( volume_header != NULL )
	{
		libluksde_volume_header_free(
		 &volume_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_volume_header_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_header_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle         = NULL;
	libcerror_error_t *error                 = NULL;
	libluksde_volume_header_t *volume_header = NULL;
	int result                               = 0;

	/* Initialize test
	 */
	result = libluksde_volume_header_initialize(
	          &volume_header,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume_header",
	 volume_header );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = luksde_test_open_file_io_handle(
	          &file_io_handle,
	          luksde_test_volume_header_data1,
	          1024,
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

	/* Test regular cases
	 */
	result = libluksde_volume_header_read_file_io_handle(
	          volume_header,
	          file_io_handle,
	          0,
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
	result = libluksde_volume_header_read_file_io_handle(
	          NULL,
	          file_io_handle,
	          0,
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

	result = libluksde_volume_header_read_file_io_handle(
	          volume_header,
	          NULL,
	          0,
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

	result = libluksde_volume_header_read_file_io_handle(
	          volume_header,
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

	/* Clean up file IO handle
	 */
	result = luksde_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data too small
	 */
	result = luksde_test_open_file_io_handle(
	          &file_io_handle,
	          luksde_test_volume_header_data1,
	          8,
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

	result = libluksde_volume_header_read_file_io_handle(
	          volume_header,
	          file_io_handle,
	          0,
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

	result = luksde_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data invalid
	 */
	result = luksde_test_open_file_io_handle(
	          &file_io_handle,
	          luksde_test_volume_header_data1,
	          1024,
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

	byte_stream_copy_from_uint64_big_endian(
	 luksde_test_volume_header_data1,
	 0xffffffffffff0001UL );

	result = libluksde_volume_header_read_file_io_handle(
	          volume_header,
	          file_io_handle,
	          0,
	          &error );

	byte_stream_copy_from_uint64_big_endian(
	 luksde_test_volume_header_data1,
	 0x4c554b53babe0001UL );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = luksde_test_close_file_io_handle(
	          &file_io_handle,
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
	result = libluksde_volume_header_free(
	          &volume_header,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume_header",
	 volume_header );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( volume_header != NULL )
	{
		libluksde_volume_header_free(
		 &volume_header,
		 NULL );
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
#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

	libcerror_error_t *error                 = NULL;
	libluksde_volume_header_t *volume_header = NULL;
	int result                               = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

	LUKSDE_TEST_UNREFERENCED_PARAMETER( argc )
	LUKSDE_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

	LUKSDE_TEST_RUN(
	 "libluksde_volume_header_initialize",
	 luksde_test_volume_header_initialize );

	LUKSDE_TEST_RUN(
	 "libluksde_volume_header_free",
	 luksde_test_volume_header_free );

	LUKSDE_TEST_RUN(
	 "libluksde_volume_header_read_data",
	 luksde_test_volume_header_read_data );

	LUKSDE_TEST_RUN(
	 "libluksde_volume_header_read_file_io_handle",
	 luksde_test_volume_header_read_file_io_handle );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libluksde_volume_header_initialize(
	          &volume_header,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume_header",
	 volume_header );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_volume_header_read_data(
	          volume_header,
	          luksde_test_volume_header_data1,
	          1024,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
/* TODO
	LUKSDE_TEST_RUN_WITH_ARGS(
	 "libluksde_volume_header_get_bytes_per_sector",
	 luksde_test_volume_header_get_bytes_per_sector,
	 volume_header );
*/

	/* Clean up
	 */
	result = libluksde_volume_header_free(
	          &volume_header,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "volume_header",
	 volume_header );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume_header != NULL )
	{
		libluksde_volume_header_free(
		 &volume_header,
		 NULL );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

