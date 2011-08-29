#include <iostream>
#include <boost/python.hpp>
#include "numpy_boost.hpp"

using namespace boost::python;

PyObject* test(const object& py_array)
{
  numpy_boost<double, 3> array(py_array.ptr());

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
}

BOOST_PYTHON_MODULE(_numpy_boost_python_test)
{
  def("test", test);

  import_array();
}
