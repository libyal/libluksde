/*
 * Library seek testing program
 *
 * Copyright (C) 2013-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include "luksde_test_libcsystem.h"
#include "luksde_test_libluksde.h"
#include "luksde_test_unused.h"

/* Define to make luksde_test_seek generate verbose output
#define LUKSDE_TEST_SEEK_VERBOSE
 */

/* Tests libluksde_volume_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int luksde_test_seek_offset(
     libluksde_volume_t *volume,
     off64_t input_offset,
     int input_whence,
     off64_t output_offset )
{
	libcerror_error_t *error  = NULL;
	const char *whence_string = NULL;
	off64_t result_offset     = 0;
	int result                = 0;

	if( volume == NULL )
	{
		return( -1 );
	}
	if( input_whence == SEEK_CUR )
	{
		whence_string = "SEEK_CUR";
	}
	else if( input_whence == SEEK_END )
	{
		whence_string = "SEEK_END";
	}
	else if( input_whence == SEEK_SET )
	{
		whence_string = "SEEK_SET";
	}
	else
	{
		whence_string = "UNKNOWN";
	}
	fprintf(
	 stdout,
	 "Testing seek of offset: %" PRIi64 " and whence: %s\t",
	 input_offset,
	 whence_string );

	result_offset = libluksde_volume_seek_offset(
	                 volume,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset == output_offset )
	{
		result = 1;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( error != NULL )
	{
		if( result != 1 )
		{
			libcerror_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Tests seeking in a volume
 * Returns 1 if successful, 0 if not or -1 on error
 */
int luksde_test_seek(
     libluksde_volume_t *volume,
     size64_t volume_size )
{
	int result = 0;

	if( volume == NULL )
	{
		return( -1 );
	}
	if( volume_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Volume size exceeds maximum.\n" );

		return( -1 );
	}
	/* Test: SEEK_SET offset: 0
	 * Expected result: 0
	 */
	result = luksde_test_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <volume_size>
	 * Expected result: <volume_size>
	 */
	result = luksde_test_seek_offset(
	          volume,
	          (off64_t) volume_size,
	          SEEK_SET,
	          (off64_t) volume_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <volume_size / 5>
	 * Expected result: <volume_size / 5>
	 */
	result = luksde_test_seek_offset(
	          volume,
	          (off64_t) ( volume_size / 5 ),
	          SEEK_SET,
	          (off64_t) ( volume_size / 5 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: <volume_size + 987>
	 * Expected result: <volume_size + 987>
	 */
	result = luksde_test_seek_offset(
	          volume,
	          (off64_t) ( volume_size + 987 ),
	          SEEK_SET,
	          (off64_t) ( volume_size + 987 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_SET offset: -987
	 * Expected result: -1
	 */
	result = luksde_test_seek_offset(
	          volume,
	          -987,
	          SEEK_SET,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: 0
	 * Expected result: <volume_size + 987>
	 */
	result = luksde_test_seek_offset(
	          volume,
	          0,
	          SEEK_CUR,
	          (off64_t) ( volume_size + 987 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: <-1 * (volume_size + 987)>
	 * Expected result: 0
	 */
	result = luksde_test_seek_offset(
	          volume,
	          -1 * (off64_t) ( volume_size + 987 ),
	          SEEK_CUR,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_CUR offset: <volume_size / 3>
	 * Expected result: <volume_size / 3>
	 */
	result = luksde_test_seek_offset(
	          volume,
	          (off64_t) ( volume_size / 3 ),
	          SEEK_CUR,
	          (off64_t) ( volume_size / 3 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	if( volume_size == 0 )
	{
		/* Test: SEEK_CUR offset: <-2 * (volume_size / 3)>
		 * Expected result: 0
		 */
		result = luksde_test_seek_offset(
		          volume,
		          -2 * (off64_t) ( volume_size / 3 ),
		          SEEK_CUR,
		          0 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset.\n" );

			return( result );
		}
	}
	else
	{
		/* Test: SEEK_CUR offset: <-2 * (volume_size / 3)>
		 * Expected result: -1
		 */
		result = luksde_test_seek_offset(
		          volume,
		          -2 * (off64_t) ( volume_size / 3 ),
		          SEEK_CUR,
		          -1 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset.\n" );

			return( result );
		}
	}
	/* Test: SEEK_END offset: 0
	 * Expected result: <volume_size>
	 */
	result = luksde_test_seek_offset(
	          volume,
	          0,
	          SEEK_END,
	          (off64_t) volume_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * volume_size>
	 * Expected result: 0
	 */
	result = luksde_test_seek_offset(
	          volume,
	          -1 * (off64_t) volume_size,
	          SEEK_END,
	          0 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * (volume_size / 4)>
	 * Expected result: <volume_size - (volume_size / 4)>
	 */
	result = luksde_test_seek_offset(
	          volume,
	          -1 * (off64_t) ( volume_size / 4 ),
	          SEEK_END,
	          (off64_t) volume_size - (off64_t) ( volume_size / 4 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: 542
	 * Expected result: <volume_size + 542>
	 */
	result = luksde_test_seek_offset(
	          volume,
	          542,
	          SEEK_END,
	          (off64_t) ( volume_size + 542 ) );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: SEEK_END offset: <-1 * (volume_size + 542)>
	 * Expected result: -1
	 */
	result = luksde_test_seek_offset(
	          volume,
	          -1 * (off64_t) ( volume_size + 542 ),
	          SEEK_END,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	/* Test: UNKNOWN (88) offset: 0
	 * Expected result: -1
	 */
	result = luksde_test_seek_offset(
	          volume,
	          0,
	          88,
	          -1 );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		return( result );
	}
	return( result );
}

/* Tests seeking in a volume
 * Returns 1 if successful, 0 if not or -1 on error
 */
int luksde_test_seek_volume(
     system_character_t *source,
     system_character_t *password,
     libcerror_error_t **error )
{
	libluksde_volume_t *volume = NULL;
	size64_t volume_size       = 0;
	size_t string_length       = 0;
	int result                 = 0;

	if( libluksde_volume_initialize(
	     &volume,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create volume.\n" );

		goto on_error;
	}
	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libluksde_volume_set_utf16_password(
		     volume,
		     (uint16_t *) password,
		     string_length,
		     error ) != 1 )
#else
		if( libluksde_volume_set_utf8_password(
		     volume,
		     (uint8_t *) password,
		     string_length,
		     error ) != 1 )
#endif
		{
			fprintf(
			 stderr,
			 "Unable to set password." );

			goto on_error;
		}
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libluksde_volume_open_wide(
	     volume,
	     source,
	     LIBLUKSDE_OPEN_READ,
	     error ) != 1 )
#else
	if( libluksde_volume_open(
	     volume,
	     source,
	     LIBLUKSDE_OPEN_READ,
	     error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open volume.\n" );

		goto on_error;
	}
	if( libluksde_volume_get_size(
	     volume,
	     &volume_size,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve volume size.\n" );

		goto on_error;
	}
	result = luksde_test_seek(
	          volume,
	          volume_size );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek in volume.\n" );

		goto on_error;
	}
	if( libluksde_volume_close(
	     volume,
	     error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close volume.\n" );

		goto on_error;
	}
	if( libluksde_volume_free(
	     &volume,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free volume.\n" );

		goto on_error;
	}
	return( result );

on_error:
	if( volume != NULL )
	{
		libluksde_volume_close(
		 volume,
		 NULL );
		libluksde_volume_free(
		 &volume,
		 NULL );
	}
	return( -1 );
}

/* Tests seeking in a volume without opening it
 * Returns 1 if successful, 0 if not or -1 on error
 */
int luksde_test_seek_volume_no_open(
     system_character_t *source LUKSDE_TEST_ATTRIBUTE_UNUSED,
     system_character_t *password,
     libcerror_error_t **error )
{
	libluksde_volume_t *volume = NULL;
	size_t string_length       = 0;
	off64_t result_offset      = 0;
	int result                 = 0;

	LUKSDE_TEST_UNREFERENCED_PARAMETER( source );

	if( libluksde_volume_initialize(
	     &volume,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create volume.\n" );

		goto on_error;
	}
	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libluksde_volume_set_utf16_password(
		     volume,
		     (uint16_t *) password,
		     string_length,
		     error ) != 1 )
#else
		if( libluksde_volume_set_utf8_password(
		     volume,
		     (uint8_t *) password,
		     string_length,
		     error ) != 1 )
#endif
		{
			fprintf(
			 stderr,
			 "Unable to set password." );

			goto on_error;
		}
	}
	fprintf(
	 stdout,
	 "Testing seek without open: \t" );

	result_offset = libluksde_volume_seek_offset(
	                 volume,
	                 0,
	                 SEEK_SET,
	                 error );

	if( result_offset == -1 )
	{
		result = 1;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( ( error != NULL )
	 && ( *error != NULL ) )
	{
		if( result != 1 )
		{
			libcerror_error_backtrace_fprint(
			 *error,
			 stderr );
		}
		libcerror_error_free(
		 error );
	}
	if( libluksde_volume_free(
	     &volume,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free volume.\n" );

		goto on_error;
	}
	return( result );

on_error:
	if( volume != NULL )
	{
		libluksde_volume_free(
		 &volume,
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error            = NULL;
	system_character_t *option_password = NULL;
	system_character_t *source          = NULL;
	system_integer_t option             = 0;
	int result                          = 0;

	while( ( option = libcsystem_getopt(
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
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

#if defined( HAVE_DEBUG_OUTPUT ) && defined( LUKSDE_TEST_SEEK_VERBOSE )
	libluksde_notify_set_verbose(
	 1 );
	libluksde_notify_set_stream(
	 stderr,
	 NULL );
#endif
	result = luksde_test_seek_volume(
	          source,
	          option_password,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek in volume.\n" );

		goto on_error;
	}
	result = luksde_test_seek_volume_no_open(
	          source,
	          option_password,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek in volume without open.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );
		libcerror_error_free(
		 &error );
	}
	return( EXIT_FAILURE );
}

