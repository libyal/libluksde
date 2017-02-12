/*
 * Signal handling functions
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

#if !defined( _LUKSDETOOLS_SIGNAL_H )
#define _LUKSDETOOLS_SIGNAL_H

#include <common.h>
#include <types.h>

#include "luksdetools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_SIGNAL_H ) && !defined( WINAPI )
#error missing signal functions
#endif

#if defined( WINAPI )
typedef unsigned long luksdetools_signal_t;

#else
typedef int luksdetools_signal_t;

#endif /* defined( WINAPI ) */

#if defined( WINAPI )

BOOL WINAPI luksdetools_signal_handler(
             luksdetools_signal_t signal );

#if defined( _MSC_VER )

void luksdetools_signal_initialize_memory_debug(
      void );

#endif /* defined( _MSC_VER ) */

#endif /* defined( WINAPI ) */

int luksdetools_signal_attach(
     void (*signal_handler)( luksdetools_signal_t ),
     libcerror_error_t **error );

int luksdetools_signal_detach(
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LUKSDETOOLS_SIGNAL_H ) */

