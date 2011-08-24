#include <iostream>
#include "numpy_boost.hpp"

extern "C" PyObject*
test(PyObject* self, PyObject* args, PyObject* kwds) {
  try {
    // Convert the incoming array to a numpy_boost object.
    // It must be convertable to DOUBLE, 3 dimensional or an
    // exception will be thrown.
    numpy_boost<double, 3> array(args);

    // The array, even if strided in a funny way, should not
    // have been copied.
    assert(args == array.py_ptr());

    // Write out the array contents using [] syntax
    std::cout << "C++" << std::endl;
    for (size_t i = 0; i < array.shape()[0]; ++i) {
      std::cout << "[";
      for (size_t j = 0; j < array.shape()[1]; ++j) {
        std::cout << "[";
        for (size_t k = 0; k < array.shape()[2]; ++k) {
          std::cout << array[i][j][k] << " ";
        }
        std::cout << "]";
      }
      std::cout << "]";
    }
    std::cout << std::endl;

    // Create a new numpy_boost array (with a new Numpy array to back
    // its memory), it fill it with some contents.
    int dims[] = { 32, 64 };
    numpy_boost<int, 2> array2(dims);

    for (size_t i = 0; i < array2.shape()[0]; ++i) {
      for (size_t j = 0; j < array2.shape()[1]; ++j) {
        array2[i][j] = i * j;
      }
    }

    // Return the Numpy array back to Python.
    PyObject* array2_obj = array2.py_ptr();
    Py_INCREF(array2_obj);
    return array2_obj;
  } catch (const python_exception& e) {
    return NULL;
  }
}

static PyMethodDef module_methods[] = {
  {"test", (PyCFunction)test, METH_O, ""},
  {NULL}
};

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
extern "C"
PyMODINIT_FUNC
init_numpy_boost_test(void) {
  PyObject* m;

  m = Py_InitModule3("_numpy_boost_test", module_methods, "");

  if (m == NULL)
    return;

  import_array();
}
