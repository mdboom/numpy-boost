#include <iostream>
#include "numpy_boost_python.hpp"

/*
  This demonstrates how to use numpy_boost with boost.python.

 */

using namespace boost::python;

numpy_boost<int, 2> test(const numpy_boost<double, 3>& array)
{
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

  return array2;
}


class ClassWithArray {
public:
  numpy_boost<double, 3> m_x;

  ClassWithArray(numpy_boost<double, 3> array) :
    m_x(array)
  {

  }

  numpy_boost<double, 3>
  get_array()
  {
    return m_x;
  }

  void
  set_array(numpy_boost<double, 3> array)
  {
    m_x = array;
  }
};


BOOST_PYTHON_MODULE(_numpy_boost_python_test)
{
  /* Initialize the Numpy support */
  import_array();

  /* You must call this function inside of the BOOST_PYTHON_MODULE
     init function to set up the type conversions.  It must be called
     for every type and dimensionality of array you intend to return.

     If you don't, the code will compile, but you will get error
     messages at runtime.
   */
  numpy_boost_python_register_type<int, 2>();
  numpy_boost_python_register_type<double, 3>();

  /* Declare a function that takes and returns an array */
  def("test", test);

  /* Declare a class with a public array member that we want to
     expose as a Python property */
  class_<ClassWithArray>("ClassWithArray", init<numpy_boost<double, 3> >())

    /* It's a shame that adding a property to a custom class is so
       convoluted in Boost.Python, but so it is...  See FAQ "Why is
       my automatic to-python conversion not being found?" */
    .add_property(
      "x",
      make_getter(&ClassWithArray::m_x, return_value_policy<return_by_value>()),
      make_setter(&ClassWithArray::m_x, return_value_policy<return_by_value>()))

    /* I think it's somewhat less "magical" to just do this */
    .add_property(
      "y", &ClassWithArray::get_array, &ClassWithArray::set_array);

}
