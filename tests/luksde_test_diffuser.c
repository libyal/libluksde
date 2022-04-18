/*
 * Library diffuser functions test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "luksde_test_libcerror.h"
#include "luksde_test_libluksde.h"
#include "luksde_test_macros.h"
#include "luksde_test_unused.h"

#include "../libluksde/libluksde_definitions.h"
#include "../libluksde/libluksde_diffuser.h"

uint8_t luksde_test_diffuser_split_data1[ 512 ] = {
	0x5e, 0x86, 0x74, 0x29, 0xeb, 0x0e, 0xab, 0x52, 0x44, 0x1f, 0x1b, 0xb0, 0x8d, 0xf2, 0x13, 0x4b,
	0xfb, 0xb3, 0xbb, 0x2f, 0x9e, 0x7c, 0x2d, 0xa9, 0x8d, 0x45, 0x34, 0x68, 0xf7, 0x1b, 0xbf, 0xc7,
	0xbc, 0x9a, 0x67, 0x96, 0x90, 0xb2, 0x59, 0x02, 0xb4, 0x9a, 0xee, 0xf9, 0x1a, 0xc2, 0xf9, 0x17,
	0x1e, 0x87, 0xdb, 0x25, 0x22, 0x17, 0x52, 0x2d, 0xa4, 0x85, 0x43, 0x38, 0xc4, 0x20, 0x32, 0xed,
	0xbc, 0xb0, 0x4d, 0x68, 0xba, 0x5c, 0x3d, 0x12, 0x5f, 0x57, 0x80, 0x52, 0xce, 0xa9, 0x55, 0x4b,
	0xf4, 0x3e, 0xed, 0xfc, 0x0d, 0x80, 0x42, 0x0c, 0x33, 0xeb, 0x14, 0x0d, 0x40, 0x69, 0xea, 0xad,
	0x08, 0x02, 0xe8, 0xc1, 0x96, 0x36, 0x5c, 0xf4, 0x97, 0x4b, 0xd6, 0xd2, 0xb2, 0x5a, 0x66, 0x91,
	0x7e, 0x1c, 0xcd, 0xa9, 0xcb, 0xd3, 0xc2, 0x8c, 0xad, 0x73, 0x7a, 0xcb, 0x30, 0x60, 0xca, 0x85,
	0xfe, 0xf1, 0x3f, 0xc9, 0x18, 0x63, 0xd8, 0xb2, 0x60, 0x6e, 0xe5, 0x02, 0xc1, 0xaf, 0x7e, 0xf6,
	0x89, 0xa3, 0x78, 0xd8, 0x77, 0x25, 0x99, 0x3e, 0x8d, 0x3e, 0xe9, 0xf5, 0x94, 0x71, 0xdb, 0x3f,
	0x19, 0x6b, 0xaa, 0xde, 0x5e, 0xec, 0xce, 0x8c, 0x60, 0x66, 0x89, 0x74, 0x5e, 0x4a, 0x1f, 0xe6,
	0xd4, 0x94, 0xad, 0x9d, 0xa2, 0x45, 0xf8, 0xd2, 0xc0, 0x3a, 0x62, 0x35, 0xd6, 0xef, 0x7d, 0xe4,
	0x00, 0xca, 0xa0, 0x86, 0xa2, 0xdd, 0xd5, 0x6f, 0x96, 0x4c, 0x61, 0x52, 0x10, 0xfe, 0x0f, 0xf6,
	0x09, 0x0a, 0x1b, 0x5f, 0x20, 0x88, 0x02, 0x90, 0x1c, 0x39, 0xdb, 0x83, 0x03, 0x4e, 0x39, 0x44,
	0xa3, 0xf5, 0x28, 0x24, 0x23, 0x4e, 0x91, 0x1d, 0x08, 0xbe, 0x62, 0x86, 0x25, 0x58, 0xa2, 0x2a,
	0x81, 0xdb, 0x8e, 0xb1, 0x38, 0xea, 0xb7, 0x2e, 0x58, 0xf6, 0xd7, 0x47, 0x8f, 0xb0, 0x9d, 0x89,
	0x3e, 0xb2, 0x46, 0x87, 0x5d, 0x91, 0xe7, 0xd5, 0xba, 0xc6, 0x52, 0x36, 0xe7, 0x1f, 0xe4, 0x24,
	0xbb, 0xa4, 0xd5, 0xaa, 0x65, 0x1f, 0x74, 0x17, 0x96, 0xe8, 0xce, 0x70, 0xbe, 0x8b, 0x42, 0x2e,
	0xb0, 0x32, 0x4b, 0x82, 0x0f, 0x94, 0xe2, 0x77, 0x5f, 0xc6, 0xe7, 0x3a, 0x58, 0x0e, 0x95, 0x25,
	0xed, 0xc5, 0x01, 0x1a, 0xaf, 0xc5, 0xdf, 0xdb, 0x72, 0x56, 0xc7, 0xf4, 0x59, 0xb4, 0xce, 0xfc,
	0x8f, 0x54, 0xc5, 0x4f, 0xad, 0x80, 0x94, 0x41, 0x46, 0x1a, 0x52, 0xa9, 0x15, 0xd8, 0x20, 0x9f,
	0x19, 0xfa, 0xfc, 0x7d, 0x33, 0x2f, 0x5f, 0x32, 0x64, 0x07, 0x33, 0x68, 0xfc, 0x57, 0x75, 0x6f,
	0x42, 0xe5, 0x07, 0x86, 0xb9, 0xa3, 0x65, 0x0f, 0x78, 0xf9, 0x22, 0x31, 0x46, 0x15, 0xc3, 0x19,
	0xa2, 0x0f, 0x59, 0xc2, 0x00, 0x6d, 0x02, 0x10, 0xaf, 0xad, 0x7d, 0x67, 0x35, 0x50, 0xf1, 0x8b,
	0xff, 0x78, 0x4d, 0x28, 0x29, 0x78, 0xc7, 0xaf, 0x5a, 0x4d, 0xa1, 0x30, 0x49, 0xee, 0xa8, 0x3f,
	0xc3, 0xd0, 0x0a, 0x11, 0xc9, 0x75, 0x84, 0x29, 0x1b, 0x2d, 0xa4, 0xdf, 0x2f, 0xd0, 0x87, 0x98,
	0x27, 0x87, 0xf5, 0xc5, 0x15, 0x2c, 0x4d, 0x8e, 0x3b, 0xbc, 0xcf, 0x6e, 0x3a, 0x2e, 0xc6, 0x1c,
	0x1c, 0x20, 0x4b, 0x78, 0x8b, 0x3c, 0x82, 0x3d, 0xf9, 0x1a, 0x93, 0x6a, 0x5a, 0xba, 0xc2, 0x29,
	0xfe, 0x8f, 0x09, 0xaa, 0xa4, 0x08, 0xa0, 0xef, 0x4c, 0x3b, 0x0c, 0xd0, 0x69, 0x06, 0x96, 0xce,
	0xe5, 0xd5, 0xe8, 0x2e, 0x08, 0x76, 0xb4, 0x98, 0x74, 0x47, 0x7c, 0x6b, 0x13, 0x49, 0xce, 0xc9,
	0x28, 0x7d, 0xde, 0x6e, 0x81, 0xf2, 0x89, 0x1d, 0x50, 0xf5, 0x0e, 0xcd, 0x1b, 0x69, 0xf9, 0xc0,
	0xad, 0x69, 0x10, 0x06, 0xe6, 0x21, 0x02, 0xee, 0xde, 0x6b, 0x1f, 0xf8, 0xd2, 0x0d, 0x5a, 0xc5 };

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

/* Tests the libluksde_diffuser_diffuse function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_diffuser_diffuse(
     void )
{
	uint8_t data[ 512 ];

	int hashing_methods[ 4 ] = {
		LIBLUKSDE_HASHING_METHOD_SHA1,
		LIBLUKSDE_HASHING_METHOD_SHA224,
		LIBLUKSDE_HASHING_METHOD_SHA256,
		LIBLUKSDE_HASHING_METHOD_SHA512 };

	libcerror_error_t *error        = NULL;
	size_t maximum_size             = 0;
	int hashing_method_index        = 0;
	int result                      = 0;

#if defined( HAVE_LUKSDE_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	for( hashing_method_index = 0;
	     hashing_method_index < 4;
	     hashing_method_index++ )
	{
		result = libluksde_diffuser_diffuse(
		          data,
		          512,
		          hashing_methods[ hashing_method_index ],
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libluksde_diffuser_diffuse(
	          NULL,
	          512,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

#if SIZEOF_SIZE_T <= 4
	maximum_size = (size_t) SSIZE_MAX;
#else
	maximum_size = (size_t) UINT32_MAX;
#endif
	result = libluksde_diffuser_diffuse(
	          data,
	          maximum_size + 1,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_diffuse(
	          data,
	          512,
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

#if defined( HAVE_LUKSDE_TEST_MEMORY )

	for( hashing_method_index = 0;
	     hashing_method_index < 4;
	     hashing_method_index++ )
	{
		for( test_number = 0;
		     test_number < number_of_malloc_fail_tests;
		     test_number++ )
		{
			/* Test libluksde_volume_header_initialize with malloc failing
			 */
			luksde_test_malloc_attempts_before_fail = test_number;

			result = libluksde_diffuser_diffuse(
			          data,
			          512,
			          hashing_methods[ hashing_method_index ],
			          &error );

			if( luksde_test_malloc_attempts_before_fail != -1 )
			{
				luksde_test_malloc_attempts_before_fail = -1;
			}
			else
			{
				LUKSDE_TEST_ASSERT_EQUAL_INT(
				 "result",
				 result,
				 -1 );

				LUKSDE_TEST_ASSERT_IS_NOT_NULL(
				 "error",
				 error );

				libcerror_error_free(
				 &error );
			}
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
	return( 0 );
}

/* Tests the libluksde_diffuser_merge function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_diffuser_merge(
     void )
{
	uint8_t data[ 32 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          512,
	          data,
	          32,
	          16,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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
	result = libluksde_diffuser_merge(
	          NULL,
	          512,
	          data,
	          32,
	          16,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          0,
	          data,
	          32,
	          16,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          (size_t) SSIZE_MAX + 1,
	          data,
	          32,
	          16,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          512,
	          NULL,
	          32,
	          16,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          512,
	          data,
	          0,
	          16,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          512,
	          data,
	          512 + 1,
	          16,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          512,
	          data,
	          32,
	          0,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          512,
	          data,
	          32,
	          128,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
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

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          512,
	          data,
	          32,
	          16,
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

#if defined( HAVE_LUKSDE_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libluksde_diffuser_merge with memset failing
	 */
	luksde_test_memset_attempts_before_fail = 1;

	result = libluksde_diffuser_merge(
	          luksde_test_diffuser_split_data1,
	          512,
	          data,
	          32,
	          16,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
	          &error );

	if( luksde_test_memset_attempts_before_fail != -1 )
	{
		luksde_test_memset_attempts_before_fail = -1;
	}
	else
	{
		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		LUKSDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_LUKSDE_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

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
	 "libluksde_diffuser_diffuse",
	 luksde_test_diffuser_diffuse );

	LUKSDE_TEST_RUN(
	 "libluksde_diffuser_merge",
	 luksde_test_diffuser_merge );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */
}

