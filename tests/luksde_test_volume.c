/*
 * Library volume type testing program
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
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "luksde_test_libcerror.h"
#include "luksde_test_libclocale.h"
#include "luksde_test_libcsystem.h"
#include "luksde_test_libluksde.h"
#include "luksde_test_libuna.h"
#include "luksde_test_macros.h"
#include "luksde_test_memory.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make luksde_test_volume generate verbose output
#define LUKSDE_TEST_VOLUME_VERBOSE
 */

/* Retrieves source as a narrow string
 * Returns 1 if successful or -1 on error
 */
int luksde_test_volume_get_narrow_source(
     const system_character_t *source,
     char *narrow_string,
     size_t narrow_string_size,
     libcerror_error_t **error )
{
	static char *function     = "luksde_test_volume_get_narrow_source";
	size_t narrow_source_size = 0;
	size_t source_length      = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( narrow_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid narrow string.",
		 function );

		return( -1 );
	}
	if( narrow_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid narrow string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow string size.",
		 function );

		return( -1 );
	}
#else
	narrow_source_size = source_length + 1;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( narrow_string_size < narrow_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: narrow string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_copy(
	     narrow_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
	narrow_string[ source_length ] = 0;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves source as a wide string
 * Returns 1 if successful or -1 on error
 */
int luksde_test_volume_get_wide_source(
     const system_character_t *source,
     wchar_t *wide_string,
     size_t wide_string_size,
     libcerror_error_t **error )
{
	static char *function   = "luksde_test_volume_get_wide_source";
	size_t source_length    = 0;
	size_t wide_source_size = 0;

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result              = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( wide_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wide string.",
		 function );

		return( -1 );
	}
	if( wide_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid wide string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_source_size = source_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine wide string size.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( wide_string_size < wide_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: wide string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     wide_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}
	wide_string[ source_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Creates and opens a source volume
 * Returns 1 if successful or -1 on error
 */
int luksde_test_volume_open_source(
     libluksde_volume_t **volume,
     const system_character_t *source,
     libcerror_error_t **error )
{
	static char *function = "luksde_test_volume_open_source";
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
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libluksde_volume_open_wide(
	          *volume,
	          source,
	          LIBLUKSDE_OPEN_READ,
	          error );
#else
	result = libluksde_volume_open(
	          *volume,
	          source,
	          LIBLUKSDE_OPEN_READ,
	          error );
#endif
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

	LUKSDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        LUKSDE_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	volume = NULL;

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
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error   = NULL;
	libluksde_volume_t *volume = NULL;
	int result                 = 0;

	/* Initialize test
	 */
	result = luksde_test_volume_get_narrow_source(
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
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error   = NULL;
	libluksde_volume_t *volume = NULL;
	int result                 = 0;

	/* Initialize test
	 */
	result = luksde_test_volume_get_wide_source(
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
     const system_character_t *source )
{
	libcerror_error_t *error   = NULL;
	libluksde_volume_t *volume = NULL;
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

/* Tests the libluksde_volume_get_offset function
 * Returns 1 if successful or 0 if not
 */
int luksde_test_volume_get_offset(
     libluksde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int offset_is_set        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libluksde_volume_get_offset(
	          volume,
	          &offset,
	          &error );

	LUKSDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset_is_set = result;

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

	if( offset_is_set != 0 )
	{
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
	}
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
	int size_is_set          = 0;

	/* Test regular cases
	 */
	result = libluksde_volume_get_size(
	          volume,
	          &size,
	          &error );

	LUKSDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	LUKSDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size_is_set = result;

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

	if( size_is_set != 0 )
	{
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
	}
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
	libcerror_error_t *error   = NULL;
	libluksde_volume_t *volume = NULL;
	system_character_t *source = NULL;
	system_integer_t option    = 0;
	int result                 = 0;

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
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
		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_open",
		 luksde_test_volume_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_open_wide",
		 luksde_test_volume_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBLUKSDE_HAVE_BFIO )

		/* TODO add test for libluksde_volume_open_file_io_handle */

#endif /* defined( LIBLUKSDE_HAVE_BFIO ) */

		LUKSDE_TEST_RUN(
		 "libluksde_volume_close",
		 luksde_test_volume_close );

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_open_close",
		 luksde_test_volume_open_close,
		 source );

		/* Initialize test
		 */
		result = luksde_test_volume_open_source(
		          &volume,
		          source,
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

#if defined( __GNUC__ )

		/* TODO: add tests for libluksde_volume_open_read */

#endif /* defined( __GNUC__ ) */

		/* TODO: add tests for libluksde_volume_read_buffer */

		/* TODO: add tests for libluksde_volume_read_buffer_at_offset */

		/* TODO: add tests for libluksde_volume_write_buffer */

		/* TODO: add tests for libluksde_volume_write_buffer_at_offset */

		/* TODO: add tests for libluksde_volume_seek_offset */

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_get_offset",
		 luksde_test_volume_get_offset,
		 volume );

		LUKSDE_TEST_RUN_WITH_ARGS(
		 "libluksde_volume_get_size",
		 luksde_test_volume_get_size,
		 volume );

		/* TODO: add tests for libluksde_volume_get_encryption_method */

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
		luksde_test_volume_close_source(
		 &volume,
		 NULL );
	}
	return( EXIT_FAILURE );
}

