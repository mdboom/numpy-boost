This code provides a very thin wrapper around `Numpy
<http://numpy.org/>`__ arrays to make them accessible in C++ as
`boost::multi_array
<http://www.boost.org/doc/libs/release/libs/multi_array/>`__
objects. Wherever possible, the data itself is not copied, even when
the data is noncontiguous.

Advantages over using the raw Numpy/C API are:

- Cleaner syntax without using macros, e.g. indexing, iterators

- Automatic integration with Python reference counting, for easier
  memory management

- Any C++ algorithm written to the ``boost::multi_array`` interface
  can easily be recompiled to work with Numpy arrays.

Known shortcomings:

- Due to the design of boost::multi_array, the datatype and number of
  dimensions of the array is fixed at compile time (though this is
  often not an important limitation in practice).

- Some features in Numpy arrays are not supported by
  ``boost::multi_array``, and therefore require an implicit data copy on
  conversion. These include:

  - Values in non-native endianess

  - Object arrays and recarrays are not supported.

.. note::

   This code is currently experimental and lacks adequate
   documentation and unit tests.

Prerequisites
-------------

- Boost 1.34 or later

- Numpy 1.1 or later

- Python 2.3 or later

Compilation
-----------

The ``setup.py`` script included in the source tree is for testing and
demonstration purposes only.  You do not need to build or install it
to start using this library.

The entirety of this project exists in a two header files
``include/numpy-boost.hpp``, which handles the bridge between Numpy
and ``boost::multi_array``, and the optional
``include/numpy-boost-python.hpp`` which is helpful if you are
wrapping your library with ``boost::python``.  These need to be on
your C++ include path, as well as Numpy's headers.  Since everything
is implemented in header files, you do not need to link to any
additional libraries.

If using distutils to build your Python extension module, this can be
achieved with the following::

  from distutils.core import setup, Extension

  import numpy
  try:
      numpy_include = numpy.get_include()
  except AttributeError:
      numpy_include = numpy.get_numpy_include()

  setup(name="my_project",
        version="0.1",
        description = "My project that uses numpy-boost",
        packages=['my_project'],
        ext_modules=[
                Extension('my_project.extension_module',
                  ['src/my_project_source.cpp'],
                  include_dirs=[PATH_TO_NUMPY_BOOST_HPP, numpy_include]
                  )
        ]
        )

Beyond that, your extension module should do all of the things
required of a Numpy-using extension module.  See the Numpy
documentation for more information.

Usage
-----

To create a ``numpy_boost`` array based on an existing Numpy array::

   PyArrayObject* numpy_array; // passed into function

   ...

   numpy_boost<double, 3> array(numpy_array);

where the template arguments to numpy_boost are the data type and the
number of dimensions.

To create a new empty ``numpy_boost`` array::

    int dims[] = { 32, 64 };
    numpy_boost<int, 2> array(dims);

To return a Numpy array back to the caller, use the ``py_ptr()``
method.  Note this returns a borrowed reference::

    PyObject* result = array.py_ptr();
    Py_INCREF(result);
    return result;
