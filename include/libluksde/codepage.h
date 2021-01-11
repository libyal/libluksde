/*
 * Codepage definitions for libluksde
 *
 * Copyright (C) 2013-2021, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBLUKSDE_CODEPAGE_H )
#define _LIBLUKSDE_CODEPAGE_H

#include <libluksde/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The codepage definitions
 */
enum LIBLUKSDE_CODEPAGES
{
	LIBLUKSDE_CODEPAGE_ASCII			= 20127,

	LIBLUKSDE_CODEPAGE_ISO_8859_1			= 28591,
	LIBLUKSDE_CODEPAGE_ISO_8859_2			= 28592,
	LIBLUKSDE_CODEPAGE_ISO_8859_3			= 28593,
	LIBLUKSDE_CODEPAGE_ISO_8859_4			= 28594,
	LIBLUKSDE_CODEPAGE_ISO_8859_5			= 28595,
	LIBLUKSDE_CODEPAGE_ISO_8859_6			= 28596,
	LIBLUKSDE_CODEPAGE_ISO_8859_7			= 28597,
	LIBLUKSDE_CODEPAGE_ISO_8859_8			= 28598,
	LIBLUKSDE_CODEPAGE_ISO_8859_9			= 28599,
	LIBLUKSDE_CODEPAGE_ISO_8859_10			= 28600,
	LIBLUKSDE_CODEPAGE_ISO_8859_11			= 28601,
	LIBLUKSDE_CODEPAGE_ISO_8859_13			= 28603,
	LIBLUKSDE_CODEPAGE_ISO_8859_14			= 28604,
	LIBLUKSDE_CODEPAGE_ISO_8859_15			= 28605,
	LIBLUKSDE_CODEPAGE_ISO_8859_16			= 28606,

	LIBLUKSDE_CODEPAGE_KOI8_R			= 20866,
	LIBLUKSDE_CODEPAGE_KOI8_U			= 21866,

	LIBLUKSDE_CODEPAGE_WINDOWS_874			= 874,
	LIBLUKSDE_CODEPAGE_WINDOWS_932			= 932,
	LIBLUKSDE_CODEPAGE_WINDOWS_936			= 936,
	LIBLUKSDE_CODEPAGE_WINDOWS_949			= 949,
	LIBLUKSDE_CODEPAGE_WINDOWS_950			= 950,
	LIBLUKSDE_CODEPAGE_WINDOWS_1250			= 1250,
	LIBLUKSDE_CODEPAGE_WINDOWS_1251			= 1251,
	LIBLUKSDE_CODEPAGE_WINDOWS_1252			= 1252,
	LIBLUKSDE_CODEPAGE_WINDOWS_1253			= 1253,
	LIBLUKSDE_CODEPAGE_WINDOWS_1254			= 1254,
	LIBLUKSDE_CODEPAGE_WINDOWS_1255			= 1255,
	LIBLUKSDE_CODEPAGE_WINDOWS_1256			= 1256,
	LIBLUKSDE_CODEPAGE_WINDOWS_1257			= 1257,
	LIBLUKSDE_CODEPAGE_WINDOWS_1258			= 1258
};

#define LIBLUKSDE_CODEPAGE_US_ASCII			LIBLUKSDE_CODEPAGE_ASCII

#define LIBLUKSDE_CODEPAGE_ISO_WESTERN_EUROPEAN		LIBLUKSDE_CODEPAGE_ISO_8859_1
#define LIBLUKSDE_CODEPAGE_ISO_CENTRAL_EUROPEAN		LIBLUKSDE_CODEPAGE_ISO_8859_2
#define LIBLUKSDE_CODEPAGE_ISO_SOUTH_EUROPEAN		LIBLUKSDE_CODEPAGE_ISO_8859_3
#define LIBLUKSDE_CODEPAGE_ISO_NORTH_EUROPEAN		LIBLUKSDE_CODEPAGE_ISO_8859_4
#define LIBLUKSDE_CODEPAGE_ISO_CYRILLIC			LIBLUKSDE_CODEPAGE_ISO_8859_5
#define LIBLUKSDE_CODEPAGE_ISO_ARABIC			LIBLUKSDE_CODEPAGE_ISO_8859_6
#define LIBLUKSDE_CODEPAGE_ISO_GREEK			LIBLUKSDE_CODEPAGE_ISO_8859_7
#define LIBLUKSDE_CODEPAGE_ISO_HEBREW			LIBLUKSDE_CODEPAGE_ISO_8859_8
#define LIBLUKSDE_CODEPAGE_ISO_TURKISH			LIBLUKSDE_CODEPAGE_ISO_8859_9
#define LIBLUKSDE_CODEPAGE_ISO_NORDIC			LIBLUKSDE_CODEPAGE_ISO_8859_10
#define LIBLUKSDE_CODEPAGE_ISO_THAI			LIBLUKSDE_CODEPAGE_ISO_8859_11
#define LIBLUKSDE_CODEPAGE_ISO_BALTIC			LIBLUKSDE_CODEPAGE_ISO_8859_13
#define LIBLUKSDE_CODEPAGE_ISO_CELTIC			LIBLUKSDE_CODEPAGE_ISO_8859_14

#define LIBLUKSDE_CODEPAGE_ISO_LATIN_1			LIBLUKSDE_CODEPAGE_ISO_8859_1
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_2			LIBLUKSDE_CODEPAGE_ISO_8859_2
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_3			LIBLUKSDE_CODEPAGE_ISO_8859_3
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_4			LIBLUKSDE_CODEPAGE_ISO_8859_4
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_5			LIBLUKSDE_CODEPAGE_ISO_8859_9
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_6			LIBLUKSDE_CODEPAGE_ISO_8859_10
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_7			LIBLUKSDE_CODEPAGE_ISO_8859_13
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_8			LIBLUKSDE_CODEPAGE_ISO_8859_14
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_9			LIBLUKSDE_CODEPAGE_ISO_8859_15
#define LIBLUKSDE_CODEPAGE_ISO_LATIN_10			LIBLUKSDE_CODEPAGE_ISO_8859_16

#define LIBLUKSDE_CODEPAGE_KOI8_RUSSIAN			LIBLUKSDE_CODEPAGE_KOI8_R
#define LIBLUKSDE_CODEPAGE_KOI8_UKRAINIAN		LIBLUKSDE_CODEPAGE_KOI8_U

#define LIBLUKSDE_CODEPAGE_WINDOWS_THAI			LIBLUKSDE_CODEPAGE_WINDOWS_874
#define LIBLUKSDE_CODEPAGE_WINDOWS_JAPANESE		LIBLUKSDE_CODEPAGE_WINDOWS_932
#define LIBLUKSDE_CODEPAGE_WINDOWS_CHINESE_SIMPLIFIED	LIBLUKSDE_CODEPAGE_WINDOWS_936
#define LIBLUKSDE_CODEPAGE_WINDOWS_KOREAN		LIBLUKSDE_CODEPAGE_WINDOWS_949
#define LIBLUKSDE_CODEPAGE_WINDOWS_CHINESE_TRADITIONAL	LIBLUKSDE_CODEPAGE_WINDOWS_950
#define LIBLUKSDE_CODEPAGE_WINDOWS_CENTRAL_EUROPEAN	LIBLUKSDE_CODEPAGE_WINDOWS_1250
#define LIBLUKSDE_CODEPAGE_WINDOWS_CYRILLIC		LIBLUKSDE_CODEPAGE_WINDOWS_1251
#define LIBLUKSDE_CODEPAGE_WINDOWS_WESTERN_EUROPEAN	LIBLUKSDE_CODEPAGE_WINDOWS_1252
#define LIBLUKSDE_CODEPAGE_WINDOWS_GREEK		LIBLUKSDE_CODEPAGE_WINDOWS_1253
#define LIBLUKSDE_CODEPAGE_WINDOWS_TURKISH		LIBLUKSDE_CODEPAGE_WINDOWS_1254
#define LIBLUKSDE_CODEPAGE_WINDOWS_HEBREW		LIBLUKSDE_CODEPAGE_WINDOWS_1255
#define LIBLUKSDE_CODEPAGE_WINDOWS_ARABIC		LIBLUKSDE_CODEPAGE_WINDOWS_1256
#define LIBLUKSDE_CODEPAGE_WINDOWS_BALTIC		LIBLUKSDE_CODEPAGE_WINDOWS_1257
#define LIBLUKSDE_CODEPAGE_WINDOWS_VIETNAMESE		LIBLUKSDE_CODEPAGE_WINDOWS_1258

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_CODEPAGE_H ) */

