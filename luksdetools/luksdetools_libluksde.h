/*
 * The libluksde header wrapper
 *
 * Copyright (C) 2013-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LUKSDETOOLS_LIBLUKSDE_H )
#define _LUKSDETOOLS_LIBLUKSDE_H

#include <common.h>

/* If Cygwin libtool DLL support is enabled set LIBLUKSDE_DLL_IMPORT
 * before including libluksde.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT ) && !defined( HAVE_STATIC_EXECUTABLES )
#define LIBLUKSDE_DLL_IMPORT
#endif

#include <libluksde.h>

#endif /* !defined( _LUKSDETOOLS_LIBLUKSDE_H ) */

