from distutils.core import setup, Extension

import numpy
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

setup(name="numpy_boost",
      version="0,1",
      description = "C++ wrappers for Numpy",
      packages=['numpy_boost'],
      ext_modules=[
        Extension('numpy_boost._numpy_boost_test',
                  ['src/_numpy_boost_test.cpp'],
                  include_dirs=['include', numpy_include],
                  undef_macros=['NDEBUG']
                  )
      ]
      )
