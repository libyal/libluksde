/*
 * LUKS anti-forensic (AF) diffuser functions
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

#if !defined( _LIBLUKSDE_DIFFUSER_H )
#define _LIBLUKSDE_DIFFUSER_H

#include <common.h>
#include <types.h>

#include "libluksde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libluksde_diffuser_diffuse(
     uint8_t *data,
     size_t data_size,
     int hashing_method,
     libcerror_error_t **error );

int libluksde_diffuser_merge(
     const uint8_t *split_data,
     size_t split_data_size,
     uint8_t *data,
     size_t data_size,
     uint32_t number_of_stripes,
     int hashing_method,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBLUKSDE_DIFFUSER_H ) */

