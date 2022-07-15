/*
 * Library sector_data type test program
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

#include "luksde_test_functions.h"
#include "luksde_test_libbfio.h"
#include "luksde_test_libcerror.h"
#include "luksde_test_libluksde.h"
#include "luksde_test_macros.h"
#include "luksde_test_memory.h"
#include "luksde_test_unused.h"

#include "../libluksde/libluksde_definitions.h"
#include "../libluksde/libluksde_encryption_context.h"
#include "../libluksde/libluksde_sector_data.h"

uint8_t luksde_test_sector_data_data1[ 512 ] = {
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c,
	0xa1, 0x7d, 0xc6, 0x4c, 0x45, 0x94, 0x17, 0x25, 0xda, 0x43, 0xfd, 0x1f, 0x6e, 0x9d, 0x87, 0x5c };

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

/* Tests the libluksde_sector_data_initialize function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_sector_data_initialize(
     void )
{
	libcerror_error_t *error             = NULL;
	libluksde_sector_data_t *sector_data = NULL;
	int result                           = 0;

#if defined( HAVE_LUKSDE_TEST_MEMORY )
	int number_of_malloc_fail_tests      = 2;
#if defined( OPTIMIZATION_DISABLED )
	int number_of_memset_fail_tests      = 1;
#endif
	int test_number                      = 0;
#endif

	/* Test regular cases
	 */
	result = libluksde_sector_data_initialize(
	          &sector_data,
	          512,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_sector_data_free(
	          &sector_data,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libluksde_sector_data_initialize(
	          NULL,
	          512,
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

	sector_data = (libluksde_sector_data_t *) 0x12345678UL;

	result = libluksde_sector_data_initialize(
	          &sector_data,
	          512,
	          &error );

	sector_data = NULL;

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_sector_data_initialize(
	          &sector_data,
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

	result = libluksde_sector_data_initialize(
	          &sector_data,
	          (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE + 1,
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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libluksde_sector_data_initialize with malloc failing
		 */
		luksde_test_malloc_attempts_before_fail = test_number;

		result = libluksde_sector_data_initialize(
		          &sector_data,
		          512,
		          &error );

		if( luksde_test_malloc_attempts_before_fail != -1 )
		{
			luksde_test_malloc_attempts_before_fail = -1;

			if( sector_data != NULL )
			{
				libluksde_sector_data_free(
				 &sector_data,
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
			 "sector_data",
			 sector_data );

			LUKSDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#if defined( OPTIMIZATION_DISABLED )

	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libluksde_sector_data_initialize with memset failing
		 */
		luksde_test_memset_attempts_before_fail = test_number;

		result = libluksde_sector_data_initialize(
		          &sector_data,
		          512,
		          &error );

		if( luksde_test_memset_attempts_before_fail != -1 )
		{
			luksde_test_memset_attempts_before_fail = -1;

			if( sector_data != NULL )
			{
				libluksde_sector_data_free(
				 &sector_data,
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
			 "sector_data",
			 sector_data );

			LUKSDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_LUKSDE_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( sector_data != NULL )
	{
		libluksde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_sector_data_free function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_sector_data_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libluksde_sector_data_free(
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

/* Tests the libluksde_sector_data_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_sector_data_read_file_io_handle(
     void )
{
	uint8_t key_data[ 32 ] = {
		0xd6, 0x7b, 0xfb, 0x28, 0x0c, 0xa0, 0xa1, 0xda, 0x36, 0x91, 0xd4, 0x3a, 0x52, 0xae, 0x4f, 0x2b,
		0x76, 0xeb, 0x90, 0x70, 0x2f, 0x60, 0x9e, 0x18, 0x70, 0xa7, 0xe6, 0x37, 0x16, 0x41, 0x60, 0x72 };

	libbfio_handle_t *file_io_handle                   = NULL;
	libcerror_error_t *error                           = NULL;
	libluksde_encryption_context_t *encryption_context = NULL;
	libluksde_sector_data_t *sector_data               = NULL;
	uint8_t *data                                      = NULL;
	int result                                         = 0;

	/* Initialize test
	 */
	result = libluksde_sector_data_initialize(
	          &sector_data,
	          512,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB,
	          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_NONE,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "encryption_context",
	 encryption_context );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_encryption_context_set_key(
	          encryption_context,
	          key_data,
	          32,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = luksde_test_open_file_io_handle(
	          &file_io_handle,
	          luksde_test_sector_data_data1,
	          512,
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
	result = libluksde_sector_data_read_file_io_handle(
	          sector_data,
	          file_io_handle,
	          0,
	          encryption_context,
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
	result = libluksde_sector_data_read_file_io_handle(
	          NULL,
	          file_io_handle,
	          0,
	          encryption_context,
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

	data              = sector_data->data;
	sector_data->data = NULL;

	result = libluksde_sector_data_read_file_io_handle(
	          sector_data,
	          file_io_handle,
	          0,
	          encryption_context,
	          0,
	          &error );

	sector_data->data = data;

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_sector_data_read_file_io_handle(
	          sector_data,
	          NULL,
	          0,
	          encryption_context,
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

	result = libluksde_sector_data_read_file_io_handle(
	          sector_data,
	          file_io_handle,
	          -1,
	          encryption_context,
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

	result = libluksde_sector_data_read_file_io_handle(
	          sector_data,
	          file_io_handle,
	          0,
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

#if defined( HAVE_LUKSDE_TEST_MEMORY )

	/* Test libluksde_sector_data_read_file_io_handle with malloc failing
	 */
	luksde_test_malloc_attempts_before_fail = 0;

	result = libluksde_sector_data_read_file_io_handle(
	          sector_data,
	          file_io_handle,
	          0,
	          encryption_context,
	          0,
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
#endif /* defined( HAVE_LUKSDE_TEST_MEMORY ) */

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
	          luksde_test_sector_data_data1,
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

	result = libluksde_sector_data_read_file_io_handle(
	          sector_data,
	          file_io_handle,
	          0,
	          encryption_context,
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

	/* Clean up
	 */
	result = libluksde_encryption_context_free(
	          &encryption_context,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "encryption_context",
	 encryption_context );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libluksde_sector_data_free(
	          &sector_data,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

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
	if( encryption_context != NULL )
	{
		libluksde_encryption_context_free(
		 &encryption_context,
		 NULL );
	}
	if( sector_data != NULL )
	{
		libluksde_sector_data_free(
		 &sector_data,
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
	LUKSDE_TEST_UNREFERENCED_PARAMETER( argc )
	LUKSDE_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

	LUKSDE_TEST_RUN(
	 "libluksde_sector_data_initialize",
	 luksde_test_sector_data_initialize );

	LUKSDE_TEST_RUN(
	 "libluksde_sector_data_free",
	 luksde_test_sector_data_free );

	LUKSDE_TEST_RUN(
	 "libluksde_sector_data_read_file_io_handle",
	 luksde_test_sector_data_read_file_io_handle );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */
}

