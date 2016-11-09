/*
 * Library support functions test program
 *
 * Copyright (C) 2013-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "luksde_test_libcerror.h"
#include "luksde_test_libluksde.h"
#include "luksde_test_macros.h"
#include "luksde_test_unused.h"

/* Tests the libluksde_get_version function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libluksde_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBLUKSDE_VERSION_STRING,
	          9 );

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libluksde_get_access_flags_read function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_get_access_flags_read(
     void )
{
	int access_flags = 0;

	access_flags = libluksde_get_access_flags_read();

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBLUKSDE_ACCESS_FLAG_READ );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libluksde_get_codepage function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_get_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libluksde_get_codepage(
	          &codepage,
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
	result = libluksde_get_codepage(
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

/* Tests the libluksde_set_codepage function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_set_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = libluksde_set_codepage(
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
	result = libluksde_set_codepage(
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
	 "libluksde_get_version",
	 luksde_test_get_version );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

