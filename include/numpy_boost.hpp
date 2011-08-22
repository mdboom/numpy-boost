/*
Copyright (c) 2008, Michael Droettboom
All rights reserved.

Licensed under the BSD license.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * The names of its contributors may not be used to endorse or
      promote products derived from this software without specific
      prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
  Documentation to come
*/

#ifndef __NUMPY_BOOST_HPP__
#define __NUMPY_BOOST_HPP__

#include <Python.h>
#include <numpy/arrayobject.h>
#include <boost/multi_array.hpp>
#include <boost/cstdint.hpp>
#include <complex>
#include <algorithm>

namespace detail {
  template<class T>
  class numpy_type_map {
  public:
    static const int typenum;
  };

  template<>
  const int numpy_type_map<float>::typenum = NPY_FLOAT;

  template<>
  const int numpy_type_map<std::complex<float> >::typenum = NPY_CFLOAT;

  template<>
  const int numpy_type_map<double>::typenum = NPY_DOUBLE;

  template<>
  const int numpy_type_map<std::complex<double> >::typenum = NPY_CDOUBLE;

  template<>
  const int numpy_type_map<long double>::typenum = NPY_LONGDOUBLE;

  template<>
  const int numpy_type_map<std::complex<long double> >::typenum = NPY_CLONGDOUBLE;

  template<>
  const int numpy_type_map<boost::int8_t>::typenum = NPY_INT8;

  template<>
  const int numpy_type_map<boost::uint8_t>::typenum = NPY_UINT8;

  template<>
  const int numpy_type_map<boost::int16_t>::typenum = NPY_INT16;

  template<>
  const int numpy_type_map<boost::uint16_t>::typenum = NPY_UINT16;

  template<>
  const int numpy_type_map<boost::int32_t>::typenum = NPY_INT32;

  template<>
  const int numpy_type_map<boost::uint32_t>::typenum = NPY_UINT32;

  template<>
  const int numpy_type_map<boost::int64_t>::typenum = NPY_INT64;

  template<>
  const int numpy_type_map<boost::uint64_t>::typenum = NPY_UINT64;
}

class numpy_boost_exception : public std::exception {

};

template<class T, int NDims>
class numpy_boost : public boost::multi_array_ref<T, NDims>
{
public:
  typedef numpy_boost<T, NDims> self_type;
  typedef boost::multi_array_ref<T, NDims> super;
  typedef typename super::size_type size_type;
  typedef T* TPtr;

private:
  PyArrayObject* array;

  void init_from_array(PyArrayObject* a) {
    array = a;
    super::base_ = (TPtr)PyArray_DATA(a);

    // It would seem like we would want to choose C or Fortran
    // ordering here based on the flags in the Numpy array.  However,
    // those flags are purely informational, the actually information
    // about storage order is recorded in the strides.
    super::storage_ = boost::c_storage_order();

    boost::detail::multi_array::copy_n(PyArray_DIMS(a), NDims, super::extent_list_.begin());
    for (size_t i = 0; i < NDims; ++i) {
      super::stride_list_[i] = PyArray_STRIDE(a, i) / sizeof(T);
    }
    std::fill_n(super::index_base_list_.begin(), NDims, 0);
    super::origin_offset_ = 0;
    super::directional_offset_ = 0;
    super::num_elements_ = std::accumulate(super::extent_list_.begin(),
                                           super::extent_list_.end(),
                                           size_type(1),
                                           std::multiplies<size_type>());
  }

public:
  numpy_boost(PyObject* obj) :
    super(NULL, std::vector<boost::uint32_t>(NDims, 0)),
    array(NULL)
  {
    PyArrayObject* a;

    a = (PyArrayObject*)PyArray_FromObject(obj, detail::numpy_type_map<T>::typenum, NDims, NDims);
    if (a == NULL) {
      // TODO: Extract Python exception
      throw numpy_boost_exception();
    }

    init_from_array(a);
  }

  numpy_boost(const self_type &other) :
    super(NULL, std::vector<boost::uint32_t>(NDims, 0)),
    array(NULL)
  {
    Py_INCREF(other.array);
    init_from_array(other.array);
  }

  template<class ExtentsList>
  explicit numpy_boost(const ExtentsList& extents) :
    super(NULL, std::vector<boost::uint32_t>(NDims, 0)),
    array(NULL)
  {
    npy_intp shape[NDims];
    PyArrayObject* a;

    boost::detail::multi_array::copy_n(extents, NDims, shape);

    a = (PyArrayObject*)PyArray_SimpleNew(NDims, shape, detail::numpy_type_map<T>::typenum);
    if (a == NULL) {
      // TODO: Extract Python exception
      throw numpy_boost_exception();
    }

    init_from_array(a);
  }

  ~numpy_boost() {
    Py_DECREF(array);
  }

  void operator=(const self_type &other) {
    Py_INCREF(other.array);
    Py_DECREF(array);
    init_from_array(other.array);
  }

  PyObject*
  py_ptr() {
    Py_INCREF(array);
    return (PyObject*)array;
  }
};

#endif
