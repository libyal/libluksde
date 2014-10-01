/*
 * Python bindings module for libluksde (pyluksde)
 *
 * Copyright (c) 2013-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyluksde.h"
#include "pyluksde_encryption_chaining_modes.h"
#include "pyluksde_encryption_methods.h"
#include "pyluksde_error.h"
#include "pyluksde_hashing_methods.h"
#include "pyluksde_initialization_vector_modes.h"
#include "pyluksde_libcerror.h"
#include "pyluksde_libcstring.h"
#include "pyluksde_libluksde.h"
#include "pyluksde_file_object_io_handle.h"
#include "pyluksde_python.h"
#include "pyluksde_unused.h"
#include "pyluksde_volume.h"

#if !defined( LIBLUKSDE_HAVE_BFIO )
LIBLUKSDE_EXTERN \
int libluksde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libluksde_error_t **error );
#endif

/* The pyluksde module methods
 */
PyMethodDef pyluksde_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pyluksde_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_volume_signature",
	  (PyCFunction) pyluksde_check_volume_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_volume_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a volume has a Linux Unified Key Setup (LUKS) Disk Encryption volume signature." },

	{ "check_volume_signature_file_object",
	  (PyCFunction) pyluksde_check_volume_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_volume_signature(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a volume has a Linux Unified Key Setup (LUKS) Disk Encryption volume signature using a file-like object." },

	{ "open",
	  (PyCFunction) pyluksde_volume_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a volume." },

	{ "open_file_object",
	  (PyCFunction) pyluksde_volume_new_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a volume using a file-like object." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pyluksde/libluksde version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyluksde_get_version(
           PyObject *self PYLUKSDE_ATTRIBUTE_UNUSED,
           PyObject *arguments PYLUKSDE_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYLUKSDE_UNREFERENCED_PARAMETER( self )
	PYLUKSDE_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libluksde_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = libcstring_narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )

/* Checks if the volume has a Linux Unified Key Setup (LUKS) Disk Encryption volume signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyluksde_check_volume_signature(
           PyObject *self PYLUKSDE_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *exception_string    = NULL;
	PyObject *exception_traceback = NULL;
	PyObject *exception_type      = NULL;
	PyObject *exception_value     = NULL;
	PyObject *string_object       = NULL;
	libcerror_error_t *error      = NULL;
	static char *function         = "pyluksde_check_volume_signature";
	static char *keyword_list[]   = { "filename", NULL };
	const wchar_t *filename_wide  = NULL;
	const char *filename_narrow   = NULL;
	char *error_string            = NULL;
	int result                    = 0;

	PYLUKSDE_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
		                    exception_value );

		error_string = PyString_AsString(
		                exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libluksde_check_volume_signature_wide(
		          filename_wide,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyluksde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check volume signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );

	if( result == -1 )
	{
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
				    exception_value );

		error_string = PyString_AsString(
				exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_narrow = PyString_AsString(
				   string_object );

		Py_BEGIN_ALLOW_THREADS

		result = libluksde_check_volume_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyluksde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check volume signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type",
	 function );

	return( NULL );
}

#else

/* Checks if the volume has a Linux Unified Key Setup (LUKS) Disk Encryption volume signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyluksde_check_volume_signature(
           PyObject *self PYLUKSDE_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyluksde_check_volume_signature";
	static char *keyword_list[] = { "filename", NULL };
	const char *filename        = NULL;
	int result                  = 0;

	PYLUKSDE_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libluksde_check_volume_signature(
	          filename,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyluksde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check volume signature.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

/* Checks if the volume has a Linux Unified Key Setup (LUKS) Disk Encryption volume signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyluksde_check_volume_signature_file_object(
           PyObject *self PYLUKSDE_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pyluksde_check_volume_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYLUKSDE_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pyluksde_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		pyluksde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libluksde_check_volume_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyluksde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check volume signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pyluksde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Declarations for DLL import/export
 */
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

/* Initializes the pyluksde module
 */
PyMODINIT_FUNC initpyluksde(
                void )
{
	PyObject *module                                      = NULL;
	PyTypeObject *encryption_chaining_modes_type_object   = NULL;
	PyTypeObject *encryption_methods_type_object          = NULL;
	PyTypeObject *hashing_methods_type_object             = NULL;
	PyTypeObject *initialization_vector_modes_type_object = NULL;
	PyTypeObject *volume_type_object                      = NULL;
	PyGILState_STATE gil_state                            = 0;

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
	module = Py_InitModule3(
	          "pyluksde",
	          pyluksde_module_methods,
	          "Python libluksde module (pyluksde)." );

	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the volume type object
	 */
	pyluksde_volume_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyluksde_volume_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyluksde_volume_type_object );

	volume_type_object = &pyluksde_volume_type_object;

	PyModule_AddObject(
	 module,
	"volume",
	(PyObject *) volume_type_object );

	/* Setup the encryption methods type object
	 */
	pyluksde_encryption_methods_type_object.tp_new = PyType_GenericNew;

	if( pyluksde_encryption_methods_init_type(
	     &pyluksde_encryption_methods_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyluksde_encryption_methods_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyluksde_encryption_methods_type_object );

	encryption_methods_type_object = &pyluksde_encryption_methods_type_object;

	PyModule_AddObject(
	 module,
	 "encryption_methods",
	 (PyObject *) encryption_methods_type_object );

	/* Setup the encryption chaining modes type object
	 */
	pyluksde_encryption_chaining_modes_type_object.tp_new = PyType_GenericNew;

	if( pyluksde_encryption_chaining_modes_init_type(
	     &pyluksde_encryption_chaining_modes_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyluksde_encryption_chaining_modes_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyluksde_encryption_chaining_modes_type_object );

	encryption_chaining_modes_type_object = &pyluksde_encryption_chaining_modes_type_object;

	PyModule_AddObject(
	 module,
	 "encryption_chaining_modes",
	 (PyObject *) encryption_chaining_modes_type_object );

	/* Setup the encryption chaining modes type object
	 */
	pyluksde_initialization_vector_modes_type_object.tp_new = PyType_GenericNew;

	if( pyluksde_initialization_vector_modes_init_type(
	     &pyluksde_initialization_vector_modes_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyluksde_initialization_vector_modes_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyluksde_initialization_vector_modes_type_object );

	initialization_vector_modes_type_object = &pyluksde_initialization_vector_modes_type_object;

	PyModule_AddObject(
	 module,
	 "initialization_vector_modes",
	 (PyObject *) initialization_vector_modes_type_object );

	/* Setup the hashing methods type object
	 */
	pyluksde_hashing_methods_type_object.tp_new = PyType_GenericNew;

	if( pyluksde_hashing_methods_init_type(
	     &pyluksde_hashing_methods_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyluksde_hashing_methods_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyluksde_hashing_methods_type_object );

	hashing_methods_type_object = &pyluksde_hashing_methods_type_object;

	PyModule_AddObject(
	 module,
	 "hashing_methods",
	 (PyObject *) hashing_methods_type_object );

on_error:
	PyGILState_Release(
	 gil_state );
}

