/*
 * Python object definition of the libluksde hashing methods
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

#if !defined( _PYLUKSDE_HASHING_METHODS_H )
#define _PYLUKSDE_HASHING_METHODS_H

#include <common.h>
#include <types.h>

#include "pyluksde_libluksde.h"
#include "pyluksde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyluksde_hashing_methods pyluksde_hashing_methods_t;

struct pyluksde_hashing_methods
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pyluksde_hashing_methods_type_object;

int pyluksde_hashing_methods_init_type(
     PyTypeObject *type_object );

PyObject *pyluksde_hashing_methods_new(
           void );

int pyluksde_hashing_methods_init(
     pyluksde_hashing_methods_t *definitions_object );

void pyluksde_hashing_methods_free(
      pyluksde_hashing_methods_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYLUKSDE_HASHING_METHODS_H ) */

