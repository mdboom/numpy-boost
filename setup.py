from distutils.core import setup, Extension

import numpy
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

print("""
NOTE: This setup.py is for testing and demonstration purposes only.
To use numpy-boost as a library, simply put numpy-boost.hpp on your
C++ include path.
""")

TEST_BOOST_PYTHON = True

extensions = []

extensions.append(
    Extension('numpy_boost._numpy_boost_test',
              ['src/_numpy_boost_test.cpp'],
              include_dirs=['include', numpy_include],
              undef_macros=['NDEBUG']
              )
    )

if TEST_BOOST_PYTHON:
    extensions.append(
        Extension('numpy_boost._numpy_boost_python_test',
                  ['src/_numpy_boost_python_test.cpp'],
                  include_dirs=['include', numpy_include],
                  undef_macros=['NDEBUG'],
                  libraries=['boost_python']
                  )
        )

setup(
    name="numpy_boost",
    version="0.1",
    description = "C++ wrappers for Numpy",
    packages=['numpy_boost'],
    ext_modules=extensions)
