/*
 * Library encryption_context type test program
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "luksde_test_libluksde.h"
#include "luksde_test_libcerror.h"
#include "luksde_test_macros.h"
#include "luksde_test_memory.h"
#include "luksde_test_unused.h"

#include "../libluksde/libluksde_definitions.h"
#include "../libluksde/libluksde_encryption_context.h"

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

/* Tests the libluksde_encryption_context_initialize function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_encryption_context_initialize(
     void )
{
	libcerror_error_t *error                           = NULL;
	libluksde_encryption_context_t *encryption_context = NULL;
	int result                                         = 0;

#if defined( HAVE_LUKSDE_TEST_MEMORY )
	int number_of_malloc_fail_tests                    = 1;
#if defined( OPTIMIZATION_DISABLED )
	int number_of_memset_fail_tests                    = 1;
#endif
	int test_number                                    = 0;
#endif

	/* Test regular cases
	 */
	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
	          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
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

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_ECB,
	          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
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

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_XTS,
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

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_ARC4,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
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

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_ARC4,
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

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_SERPENT,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
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

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_SERPENT,
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

	/* Test error cases
	 */
	result = libluksde_encryption_context_initialize(
	          NULL,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
	          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
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

	encryption_context = (libluksde_encryption_context_t *) 0x12345678UL;

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
	          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
	          LIBLUKSDE_HASHING_METHOD_SHA1,
	          &error );

	encryption_context = NULL;

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          -1,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
	          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
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

#if defined( HAVE_LUKSDE_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libluksde_encryption_context_initialize with malloc failing
		 */
		luksde_test_malloc_attempts_before_fail = test_number;

		result = libluksde_encryption_context_initialize(
		          &encryption_context,
		          LIBLUKSDE_ENCRYPTION_METHOD_AES,
		          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
		          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
		          LIBLUKSDE_HASHING_METHOD_SHA1,
		          &error );

		if( luksde_test_malloc_attempts_before_fail != -1 )
		{
			luksde_test_malloc_attempts_before_fail = -1;

			if( encryption_context != NULL )
			{
				libluksde_encryption_context_free(
				 &encryption_context,
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
			 "encryption_context",
			 encryption_context );

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
		/* Test libluksde_encryption_context_initialize with memset failing
		 */
		luksde_test_memset_attempts_before_fail = test_number;

		result = libluksde_encryption_context_initialize(
		          &encryption_context,
		          LIBLUKSDE_ENCRYPTION_METHOD_AES,
		          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
		          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
		          LIBLUKSDE_HASHING_METHOD_SHA1,
		          &error );

		if( luksde_test_memset_attempts_before_fail != -1 )
		{
			luksde_test_memset_attempts_before_fail = -1;

			if( encryption_context != NULL )
			{
				libluksde_encryption_context_free(
				 &encryption_context,
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
			 "encryption_context",
			 encryption_context );

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
	if( encryption_context != NULL )
	{
		libluksde_encryption_context_free(
		 &encryption_context,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_encryption_context_free function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_encryption_context_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libluksde_encryption_context_free(
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

/* Tests the libluksde_encryption_context_set_key function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_encryption_context_set_key(
     void )
{
	uint8_t key_data[ 32 ] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

	libcerror_error_t *error                           = NULL;
	libluksde_encryption_context_t *encryption_context = NULL;
	int result                                         = 0;

	/* Initialize test
	 */
	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
	          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
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

	/* Test regular cases
	 */
	result = libluksde_encryption_context_set_key(
	          encryption_context,
	          key_data,
	          16,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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

	/* Initialize test
	 */
	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_XTS,
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

	/* Test regular cases
	 */
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

	/* Initialize test
	 */
	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_ARC4,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
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

	/* Test regular cases
	 */
	result = libluksde_encryption_context_set_key(
	          encryption_context,
	          key_data,
	          16,
	          &error );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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

	/* Initialize test
	 */
	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_SERPENT,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
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

	/* Test regular cases
	 */
	result = libluksde_encryption_context_set_key(
	          encryption_context,
	          key_data,
	          16,
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
	result = libluksde_encryption_context_set_key(
	          NULL,
	          key_data,
	          16,
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

	result = libluksde_encryption_context_set_key(
	          encryption_context,
	          NULL,
	          16,
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

	result = libluksde_encryption_context_set_key(
	          encryption_context,
	          key_data,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( encryption_context != NULL )
	{
		libluksde_encryption_context_free(
		 &encryption_context,
		 NULL );
	}
	return( 0 );
}

/* Tests the libluksde_encryption_context_crypt function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_encryption_context_crypt(
     void )
{
	uint8_t encrypted_data[ 128 ];
	uint8_t data[ 128 ];

	libcerror_error_t *error                           = NULL;
	libluksde_encryption_context_t *encryption_context = NULL;
	int result                                         = 0;

	memory_set(
	 encrypted_data,
	 0,
	 128 );

	/* Initialize test
	 */
	result = libluksde_encryption_context_initialize(
	          &encryption_context,
	          LIBLUKSDE_ENCRYPTION_METHOD_AES,
	          LIBLUKSDE_ENCRYPTION_CHAINING_MODE_CBC,
	          LIBLUKSDE_INITIALIZATION_VECTOR_MODE_ESSIV,
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

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libluksde_encryption_context_crypt(
	          NULL,
	          LIBLUKSDE_ENCRYPTION_CRYPT_MODE_DECRYPT,
	          encrypted_data,
	          128,
	          data,
	          128,
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

	result = libluksde_encryption_context_crypt(
	          encryption_context,
	          -1,
	          encrypted_data,
	          128,
	          data,
	          128,
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

	result = libluksde_encryption_context_crypt(
	          encryption_context,
	          LIBLUKSDE_ENCRYPTION_CRYPT_MODE_DECRYPT,
	          encrypted_data,
	          0,
	          data,
	          128,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( encryption_context != NULL )
	{
		libluksde_encryption_context_free(
		 &encryption_context,
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
	 "libluksde_encryption_context_initialize",
	 luksde_test_encryption_context_initialize );

	LUKSDE_TEST_RUN(
	 "libluksde_encryption_context_free",
	 luksde_test_encryption_context_free );

	LUKSDE_TEST_RUN(
	 "libluksde_encryption_context_set_key",
	 luksde_test_encryption_context_set_key );

	LUKSDE_TEST_RUN(
	 "libluksde_encryption_context_crypt",
	 luksde_test_encryption_context_crypt );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */
}

