/*
 * Python object definition of the libluksde initialization vector modes
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

#if !defined( _PYLUKSDE_INITIALIZATION_VECTOR_MODES_H )
#define _PYLUKSDE_INITIALIZATION_VECTOR_MODES_H

#include <common.h>
#include <types.h>

#include "pyluksde_libluksde.h"
#include "pyluksde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyluksde_initialization_vector_modes pyluksde_initialization_vector_modes_t;

struct pyluksde_initialization_vector_modes
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pyluksde_initialization_vector_modes_type_object;

int pyluksde_initialization_vector_modes_init_type(
     PyTypeObject *type_object );

PyObject *pyluksde_initialization_vector_modes_new(
           void );

int pyluksde_initialization_vector_modes_init(
     pyluksde_initialization_vector_modes_t *definitions_object );

void pyluksde_initialization_vector_modes_free(
      pyluksde_initialization_vector_modes_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYLUKSDE_INITIALIZATION_VECTOR_MODES_H ) */

