/*
 * Library password functions test program
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

#include "../libluksde/libluksde_definitions.h"
#include "../libluksde/libluksde_password.h"

typedef struct luksde_test_password_pbkdf2_test_vector luksde_test_password_pbkdf2_test_vector_t;

struct luksde_test_password_pbkdf2_test_vector
{
	/* The description
	 */
	const char *description;

	/* The password hashing method
	 */
	int password_hashing_method;

	/* The number of iterations
	 */
	uint32_t number_of_iterations;

	/* The password
	 */
	char *password;

	/* The password length
	 */
	size_t password_length;

	/* The salt
	 */
	uint8_t salt[ 48 ];

	/* The salt size
	 */
	size_t salt_size;

	/* The expected output data
	 */
	uint8_t output_data[ 64 ];

	/* The expected output data size
	 */
	size_t output_data_size;
};

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

/* Tests the libluksde_password_pbkdf2 function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_password_pbkdf2(
     void )
{
	uint8_t output_data[ 64 ];

	luksde_test_password_pbkdf2_test_vector_t test_vectors[ 25 ] = {
		/* RFC 3962 test vectors
		 */
		{ "RFC 3962 test vector 1 SHA-1 16 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1, "password", 8,
		  { 'A', 'T', 'H', 'E', 'N', 'A', '.', 'M', 'I', 'T', '.', 'E', 'D', 'U', 'r', 'a', 'e', 'b', 'u', 'r', 'n' }, 21,
		  { 0xcd, 0xed, 0xb5, 0x28, 0x1b, 0xb2, 0xf8, 0x01, 0x56, 0x5a, 0x11, 0x22, 0xb2, 0x56, 0x35, 0x15 }, 16 },
		{ "RFC 3962 test vector 2 SHA-1 16 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 2, "password", 8,
		  { 'A', 'T', 'H', 'E', 'N', 'A', '.', 'M', 'I', 'T', '.', 'E', 'D', 'U', 'r', 'a', 'e', 'b', 'u', 'r', 'n' }, 21,
		  { 0x01, 0xdb, 0xee, 0x7f, 0x4a, 0x9e, 0x24, 0x3e, 0x98, 0x8b, 0x62, 0xc7, 0x3c, 0xda, 0x93, 0x5d }, 16 },
		{ "RFC 3962 test vector 3 SHA-1 16 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1200, "password", 8,
		  { 'A', 'T', 'H', 'E', 'N', 'A', '.', 'M', 'I', 'T', '.', 'E', 'D', 'U', 'r', 'a', 'e', 'b', 'u', 'r', 'n' }, 21,
		  { 0x5c, 0x08, 0xeb, 0x61, 0xfd, 0xf7, 0x1e, 0x4e, 0x4e, 0xc3, 0xcf, 0x6b, 0xa1, 0xf5, 0x51, 0x2b }, 16 },
		{ "RFC 3962 test vector 4 SHA-1 16 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 5, "password", 8,
		  { 0x12, 0x34, 0x56, 0x78, 0x78, 0x56, 0x34, 0x12 }, 8,
		  { 0xd1, 0xda, 0xa7, 0x86, 0x15, 0xf2, 0x87, 0xe6, 0xa1, 0xc8, 0xb1, 0x20, 0xd7, 0x06, 0x2a, 0x49 }, 16 },
		{ "RFC 3962 test vector 5 SHA-1 16 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1200, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", 64,
		  { 'p', 'a', 's', 's', ' ', 'p', 'h', 'r', 'a', 's', 'e', ' ', 'e', 'q', 'u', 'a', 'l', 's', ' ', 'b', 'l', 'o', 'c', 'k', ' ', 's', 'i', 'z', 'e' }, 29,
		  { 0x13, 0x9c, 0x30, 0xc0, 0x96, 0x6b, 0xc3, 0x2b, 0xa5, 0x5f, 0xdb, 0xf2, 0x12, 0x53, 0x0a, 0xc9 }, 16 },
		{ "RFC 3962 test vector 6 SHA-1 16 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1200, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", 65,
		  { 'p', 'a', 's', 's', ' ', 'p', 'h', 'r', 'a', 's', 'e', ' ', 'e', 'x', 'c', 'e', 'e', 'd', 's', ' ', 'b', 'l', 'o', 'c', 'k', ' ', 's', 'i', 'z', 'e' }, 30,
		  { 0x9c, 0xca, 0xd6, 0xd4, 0x68, 0x77, 0x0c, 0xd5, 0x1b, 0x10, 0xe6, 0xa6, 0x87, 0x21, 0xbe, 0x61 }, 16 },
		{ "RFC 3962 test vector 7 SHA-1 16 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 50, "\xf0\x9d\x84\x9e", 4,
		  { 'E', 'X', 'A', 'M', 'P', 'L', 'E', '.', 'C', 'O', 'M', 'p', 'i', 'a', 'n', 'i', 's', 't' }, 18,
		  { 0x6b, 0x9c, 0xf2, 0x6d, 0x45, 0x45, 0x5a, 0x43, 0xa5, 0xb8, 0xbb, 0x27, 0x6a, 0x40, 0x3b, 0x39 }, 16 },

		{ "RFC 3962 test vector 1 SHA-1 32 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1, "password", 8,
		  { 'A', 'T', 'H', 'E', 'N', 'A', '.', 'M', 'I', 'T', '.', 'E', 'D', 'U', 'r', 'a', 'e', 'b', 'u', 'r', 'n' }, 21,
		  { 0xcd, 0xed, 0xb5, 0x28, 0x1b, 0xb2, 0xf8, 0x01, 0x56, 0x5a, 0x11, 0x22, 0xb2, 0x56, 0x35, 0x15, 0x0a, 0xd1, 0xf7, 0xa0, 0x4b, 0xb9, 0xf3, 0xa3, 0x33, 0xec, 0xc0, 0xe2, 0xe1, 0xf7, 0x08, 0x37 }, 32 },
		{ "RFC 3962 test vector 2 SHA-1 32 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 2, "password", 8,
		  { 'A', 'T', 'H', 'E', 'N', 'A', '.', 'M', 'I', 'T', '.', 'E', 'D', 'U', 'r', 'a', 'e', 'b', 'u', 'r', 'n' }, 21,
		  { 0x01, 0xdb, 0xee, 0x7f, 0x4a, 0x9e, 0x24, 0x3e, 0x98, 0x8b, 0x62, 0xc7, 0x3c, 0xda, 0x93, 0x5d, 0xa0, 0x53, 0x78, 0xb9, 0x32, 0x44, 0xec, 0x8f, 0x48, 0xa9, 0x9e, 0x61, 0xad, 0x79, 0x9d, 0x86 }, 32 },
		{ "RFC 3962 test vector 3 SHA-1 32 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1200, "password", 8,
		  { 'A', 'T', 'H', 'E', 'N', 'A', '.', 'M', 'I', 'T', '.', 'E', 'D', 'U', 'r', 'a', 'e', 'b', 'u', 'r', 'n' }, 21,
		  { 0x5c, 0x08, 0xeb, 0x61, 0xfd, 0xf7, 0x1e, 0x4e, 0x4e, 0xc3, 0xcf, 0x6b, 0xa1, 0xf5, 0x51, 0x2b, 0xa7, 0xe5, 0x2d, 0xdb, 0xc5, 0xe5, 0x14, 0x2f, 0x70, 0x8a, 0x31, 0xe2, 0xe6, 0x2b, 0x1e, 0x13 }, 32 },
		{ "RFC 3962 test vector 4 SHA-1 32 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 5, "password", 8,
		  { 0x12, 0x34, 0x56, 0x78, 0x78, 0x56, 0x34, 0x12 }, 8,
		  { 0xd1, 0xda, 0xa7, 0x86, 0x15, 0xf2, 0x87, 0xe6, 0xa1, 0xc8, 0xb1, 0x20, 0xd7, 0x06, 0x2a, 0x49, 0x3f, 0x98, 0xd2, 0x03, 0xe6, 0xbe, 0x49, 0xa6, 0xad, 0xf4, 0xfa, 0x57, 0x4b, 0x6e, 0x64, 0xee }, 32 },
		{ "RFC 3962 test vector 5 SHA-1 32 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1200, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", 64,
		  { 'p', 'a', 's', 's', ' ', 'p', 'h', 'r', 'a', 's', 'e', ' ', 'e', 'q', 'u', 'a', 'l', 's', ' ', 'b', 'l', 'o', 'c', 'k', ' ', 's', 'i', 'z', 'e' }, 29,
		  { 0x13, 0x9c, 0x30, 0xc0, 0x96, 0x6b, 0xc3, 0x2b, 0xa5, 0x5f, 0xdb, 0xf2, 0x12, 0x53, 0x0a, 0xc9, 0xc5, 0xec, 0x59, 0xf1, 0xa4, 0x52, 0xf5, 0xcc, 0x9a, 0xd9, 0x40, 0xfe, 0xa0, 0x59, 0x8e, 0xd1 }, 32 },
		{ "RFC 3962 test vector 6 SHA-1 32 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1200, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", 65,
		  { 'p', 'a', 's', 's', ' ', 'p', 'h', 'r', 'a', 's', 'e', ' ', 'e', 'x', 'c', 'e', 'e', 'd', 's', ' ', 'b', 'l', 'o', 'c', 'k', ' ', 's', 'i', 'z', 'e' }, 30,
		  { 0x9c, 0xca, 0xd6, 0xd4, 0x68, 0x77, 0x0c, 0xd5, 0x1b, 0x10, 0xe6, 0xa6, 0x87, 0x21, 0xbe, 0x61, 0x1a, 0x8b, 0x4d, 0x28, 0x26, 0x01, 0xdb, 0x3b, 0x36, 0xbe, 0x92, 0x46, 0x91, 0x5e, 0xc8, 0x2a }, 32 },
		{ "RFC 3962 test vector 7 SHA-1 32 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 50, "\xf0\x9d\x84\x9e", 4,
		  { 'E', 'X', 'A', 'M', 'P', 'L', 'E', '.', 'C', 'O', 'M', 'p', 'i', 'a', 'n', 'i', 's', 't' }, 18,
		  { 0x6b, 0x9c, 0xf2, 0x6d, 0x45, 0x45, 0x5a, 0x43, 0xa5, 0xb8, 0xbb, 0x27, 0x6a, 0x40, 0x3b, 0x39, 0xe7, 0xfe, 0x37, 0xa0, 0xc4, 0x1e, 0x02, 0xc2, 0x81, 0xff, 0x30, 0x69, 0xe1, 0xe9, 0x4f, 0x52 }, 32 },

		/* RFC 6070 test vectors
		 */
		{ "RFC 6070 test vector 1 SHA-1 20 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 1, "password", 8,
		  { 's', 'a', 'l', 't' }, 4,
		  { 0x0c, 0x60, 0xc8, 0x0f, 0x96, 0x1f, 0x0e, 0x71, 0xf3, 0xa9, 0xb5, 0x24, 0xaf, 0x60, 0x12, 0x06, 0x2f, 0xe0, 0x37, 0xa6 }, 20 },
		{ "RFC 6070 test vector 2 SHA-1 20 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 2, "password", 8,
		  { 's', 'a', 'l', 't' }, 4,
		  { 0xea, 0x6c, 0x01, 0x4d, 0xc7, 0x2d, 0x6f, 0x8c, 0xcd, 0x1e, 0xd9, 0x2a, 0xce, 0x1d, 0x41, 0xf0, 0xd8, 0xde, 0x89, 0x57 }, 20 },
		{ "RFC 6070 test vector 3 SHA-1 20 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 4096, "password", 8,
		  { 's', 'a', 'l', 't' }, 4,
		  { 0x4b, 0x00, 0x79, 0x01, 0xb7, 0x65, 0x48, 0x9a, 0xbe, 0xad, 0x49, 0xd9, 0x26, 0xf7, 0x21, 0xd0, 0x65, 0xa4, 0x29, 0xc1 }, 20 },
		{ "RFC 6070 test vector 4 SHA-1 20 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 16777216, "password", 8,
		  { 's', 'a', 'l', 't' }, 4,
		  { 0xee, 0xfe, 0x3d, 0x61, 0xcd, 0x4d, 0xa4, 0xe4, 0xe9, 0x94, 0x5b, 0x3d, 0x6b, 0xa2, 0x15, 0x8c, 0x26, 0x34, 0xe9, 0x84 }, 20 },
		{ "RFC 6070 test vector 5 SHA-1 25 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 4096, "passwordPASSWORDpassword", 24,
		  { 's', 'a', 'l', 't', 'S', 'A', 'L', 'T', 's', 'a', 'l', 't', 'S', 'A', 'L', 'T', 's', 'a', 'l', 't', 'S', 'A', 'L', 'T', 's', 'a', 'l', 't', 'S', 'A', 'L', 'T', 's', 'a', 'l', 't' }, 36,
		  { 0x3d, 0x2e, 0xec, 0x4f, 0xe4, 0x1c, 0x84, 0x9b, 0x80, 0xc8, 0xd8, 0x36, 0x62, 0xc0, 0xe4, 0x4a, 0x8b, 0x29, 0x1a, 0x96, 0x4c, 0xf2, 0xf0, 0x70, 0x38 }, 25 },
		{ "RFC 6070 test vector 6 SHA-1 16 byte output", LIBLUKSDE_HASHING_METHOD_SHA1, 4096, "pass\0word", 9,
		  { 's', 'a', 0, 'l', 't' }, 5,
		  { 0x56, 0xfa, 0x6a, 0xa7, 0x55, 0x48, 0x09, 0x9d, 0xcc, 0x37, 0xd7, 0xf0, 0x34, 0x25, 0xe0, 0xc3 }, 16 },

		{ "RFC 6070 test vector 1 SHA-224 28 byte output", LIBLUKSDE_HASHING_METHOD_SHA224, 1, "password", 8,
		  { 's', 'a', 'l', 't' }, 4,
		  { 0x3c, 0x19, 0x8c, 0xbd, 0xb9, 0x46, 0x4b, 0x78, 0x57, 0x96, 0x6b, 0xd0, 0x5b, 0x7b, 0xc9, 0x2b, 0xc1, 0xcc, 0x4e, 0x6e, 0x63, 0x15, 0x5d, 0x4e, 0x49, 0x05, 0x57, 0xfd }, 24 },

		{ "RFC 6070 test vector 1 SHA-256 32 byte output", LIBLUKSDE_HASHING_METHOD_SHA256, 1, "password", 8,
		  { 's', 'a', 'l', 't' }, 4,
		  { 0x12, 0x0f, 0xb6, 0xcf, 0xfc, 0xf8, 0xb3, 0x2c, 0x43, 0xe7, 0x22, 0x52, 0x56, 0xc4, 0xf8, 0x37, 0xa8, 0x65, 0x48, 0xc9, 0x2c, 0xcc, 0x35, 0x48, 0x08, 0x05, 0x98, 0x7c, 0xb7, 0x0b, 0xe1, 0x7b }, 32 },

		{ "RFC 6070 test vector 1 SHA-512 64 byte output", LIBLUKSDE_HASHING_METHOD_SHA512, 1, "password", 8,
		  { 's', 'a', 'l', 't' }, 4,
		  { 0x86, 0x7f, 0x70, 0xcf, 0x1a, 0xde, 0x02, 0xcf, 0xf3, 0x75, 0x25, 0x99, 0xa3, 0xa5, 0x3d, 0xc4, 0xaf, 0x34, 0xc7, 0xa6, 0x69, 0x81, 0x5a, 0xe5, 0xd5, 0x13, 0x55, 0x4e, 0x1c, 0x8c, 0xf2, 0x52, 0xc0, 0x2d, 0x47, 0x0a, 0x28, 0x5a, 0x05, 0x01, 0xba, 0xd9, 0x99, 0xbf, 0xe9, 0x43, 0xc0, 0x8f, 0x05, 0x02, 0x35, 0xd7, 0xd6, 0x8b, 0x1d, 0xa5, 0x5e, 0x63, 0xf7, 0x3b, 0x60, 0xa5, 0x7f, 0xce }, 64 },

		/* RFC 7914 test vectors
		 */
		{ "RFC 7914 test vector 1 SHA-256 64 byte output", LIBLUKSDE_HASHING_METHOD_SHA256, 1, "passwd", 6,
		  { 's', 'a', 'l', 't' }, 4,
		  { 0x55, 0xac, 0x04, 0x6e, 0x56, 0xe3, 0x08, 0x9f, 0xec, 0x16, 0x91, 0xc2, 0x25, 0x44, 0xb6, 0x05, 0xf9, 0x41, 0x85, 0x21, 0x6d, 0xde, 0x04, 0x65, 0xe6, 0x8b, 0x9d, 0x57, 0xc2, 0x0d, 0xac, 0xbc, 0x49, 0xca, 0x9c, 0xcc, 0xf1, 0x79, 0xb6, 0x45, 0x99, 0x16, 0x64, 0xb3, 0x9d, 0x77, 0xef, 0x31, 0x7c, 0x71, 0xb8, 0x45, 0xb1, 0xe3, 0x0b, 0xd5, 0x09, 0x11, 0x20, 0x41, 0xd3, 0xa1, 0x97, 0x83 }, 64 },
		{ "RFC 7914 test vector 1 SHA-256 64 byte output", LIBLUKSDE_HASHING_METHOD_SHA256, 80000, "Password", 8,
		  { 'N', 'a', 'C', 'l' }, 4,
		  { 0x4d, 0xdc, 0xd8, 0xf6, 0x0b, 0x98, 0xbe, 0x21, 0x83, 0x0c, 0xee, 0x5e, 0xf2, 0x27, 0x01, 0xf9, 0x64, 0x1a, 0x44, 0x18, 0xd0, 0x4c, 0x04, 0x14, 0xae, 0xff, 0x08, 0x87, 0x6b, 0x34, 0xab, 0x56, 0xa1, 0xd4, 0x25, 0xa1, 0x22, 0x58, 0x33, 0x54, 0x9a, 0xdb, 0x84, 0x1b, 0x51, 0xc9, 0xb3, 0x17, 0x6a, 0x27, 0x2b, 0xde, 0xbb, 0xa1, 0xd0, 0x78, 0x47, 0x8f, 0x62, 0xb3, 0x97, 0xf3, 0x3c, 0x8d }, 64 },
	};

	libcerror_error_t *error        = NULL;
	int result                      = 0;
	int test_number                 = 0;

#if defined( HAVE_LUKSDE_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )
	int number_of_memset_fail_tests = 3;
#endif

	/* Test regular
	 */
	for( test_number = 0;
	     test_number < 25;
	     test_number++ )
	{
		result = libluksde_password_pbkdf2(
		          (uint8_t *) test_vectors[ test_number ].password,
		          test_vectors[ test_number ].password_length,
		          test_vectors[ test_number ].password_hashing_method,
		          test_vectors[ test_number ].salt,
		          test_vectors[ test_number ].salt_size,
		          test_vectors[ test_number ].number_of_iterations,
		          output_data,
		          test_vectors[ test_number ].output_data_size,
		          &error );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		LUKSDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = memory_compare(
		          output_data,
		          test_vectors[ test_number ].output_data,
		          test_vectors[ test_number ].output_data_size );

		LUKSDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );
	}
	/* Test error cases
	 */
	result = libluksde_password_pbkdf2(
	          NULL,
	          test_vectors[ 0 ].password_length,
	          test_vectors[ 0 ].password_hashing_method,
	          test_vectors[ 0 ].salt,
	          test_vectors[ 0 ].salt_size,
	          test_vectors[ 0 ].number_of_iterations,
	          output_data,
	          test_vectors[ 0 ].output_data_size,
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

	result = libluksde_password_pbkdf2(
	          (uint8_t *) test_vectors[ 0 ].password,
	          (size_t) SSIZE_MAX + 1,
	          test_vectors[ 0 ].password_hashing_method,
	          test_vectors[ 0 ].salt,
	          test_vectors[ 0 ].salt_size,
	          test_vectors[ 0 ].number_of_iterations,
	          output_data,
	          test_vectors[ 0 ].output_data_size,
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

	result = libluksde_password_pbkdf2(
	          (uint8_t *) test_vectors[ 0 ].password,
	          test_vectors[ 0 ].password_length,
	          -1,
	          test_vectors[ 0 ].salt,
	          test_vectors[ 0 ].salt_size,
	          test_vectors[ 0 ].number_of_iterations,
	          output_data,
	          test_vectors[ 0 ].output_data_size,
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

	result = libluksde_password_pbkdf2(
	          (uint8_t *) test_vectors[ 0 ].password,
	          test_vectors[ 0 ].password_length,
	          test_vectors[ 0 ].password_hashing_method,
	          NULL,
	          test_vectors[ 0 ].salt_size,
	          test_vectors[ 0 ].number_of_iterations,
	          output_data,
	          test_vectors[ 0 ].output_data_size,
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

	result = libluksde_password_pbkdf2(
	          (uint8_t *) test_vectors[ 0 ].password,
	          test_vectors[ 0 ].password_length,
	          test_vectors[ 0 ].password_hashing_method,
	          test_vectors[ 0 ].salt,
	          (size_t) SSIZE_MAX + 1,
	          test_vectors[ 0 ].number_of_iterations,
	          output_data,
	          test_vectors[ 0 ].output_data_size,
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

	result = libluksde_password_pbkdf2(
	          (uint8_t *) test_vectors[ 0 ].password,
	          test_vectors[ 0 ].password_length,
	          test_vectors[ 0 ].password_hashing_method,
	          test_vectors[ 0 ].salt,
	          test_vectors[ 0 ].salt_size,
	          0,
	          output_data,
	          test_vectors[ 0 ].output_data_size,
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

	result = libluksde_password_pbkdf2(
	          (uint8_t *) test_vectors[ 0 ].password,
	          test_vectors[ 0 ].password_length,
	          test_vectors[ 0 ].password_hashing_method,
	          test_vectors[ 0 ].salt,
	          test_vectors[ 0 ].salt_size,
	          test_vectors[ 0 ].number_of_iterations,
	          NULL,
	          test_vectors[ 0 ].output_data_size,
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

	result = libluksde_password_pbkdf2(
	          (uint8_t *) test_vectors[ 0 ].password,
	          test_vectors[ 0 ].password_length,
	          test_vectors[ 0 ].password_hashing_method,
	          test_vectors[ 0 ].salt,
	          test_vectors[ 0 ].salt_size,
	          test_vectors[ 0 ].number_of_iterations,
	          output_data,
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

#if defined( HAVE_LUKSDE_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libluksde_password_pbkdf2 with memset failing
		 */
		luksde_test_memset_attempts_before_fail = test_number;

		result = libluksde_password_pbkdf2(
		          (uint8_t *) test_vectors[ 0 ].password,
		          test_vectors[ 0 ].password_length,
		          test_vectors[ 0 ].password_hashing_method,
		          test_vectors[ 0 ].salt,
		          test_vectors[ 0 ].salt_size,
		          test_vectors[ 0 ].number_of_iterations,
		          output_data,
		          test_vectors[ 0 ].output_data_size,
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
	 "libluksde_password_pbkdf2",
	 luksde_test_password_pbkdf2 );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBLUKSDE_DLL_IMPORT ) */
}

