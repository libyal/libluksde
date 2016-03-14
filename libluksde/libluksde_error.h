/*
 * Error functions
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

#if !defined( _LIBLUKSDE_INTERNAL_ERROR_H )
#define _LIBLUKSDE_INTERNAL_ERROR_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if !defined( HAVE_LOCAL_LIBLUKSDE )
#include <libluksde/error.h>
#endif

#include "libluksde_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBLUKSDE )

LIBLUKSDE_EXTERN \
void libluksde_error_free(
      libluksde_error_t **error );

LIBLUKSDE_EXTERN \
int libluksde_error_fprint(
     libluksde_error_t *error,
     FILE *stream );

LIBLUKSDE_EXTERN \
int libluksde_error_sprint(
     libluksde_error_t *error,
     char *string,
     size_t size );

LIBLUKSDE_EXTERN \
int libluksde_error_backtrace_fprint(
     libluksde_error_t *error,
     FILE *stream );

LIBLUKSDE_EXTERN \
int libluksde_error_backtrace_sprint(
     libluksde_error_t *error,
     char *string,
     size_t size );

#endif /* !defined( HAVE_LOCAL_LIBLUKSDE ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_INTERNAL_ERROR_H ) */

