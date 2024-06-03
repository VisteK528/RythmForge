#ifndef RYTHMFORGE_NP_BOOST_ARRAY_HPP
#define RYTHMFORGE_NP_BOOST_ARRAY_HPP
#include "pybind11/pybind11.h"
#include <pybind11/numpy.h>
#include <boost/multi_array.hpp>
#include <memory>
namespace py = pybind11;

 std::unique_ptr<boost::multi_array<double,2>>np2DtoMultiarray(const py::array& npArray);
 py::array_t<double> multiarray2DtoNp(const std::unique_ptr<boost::multi_array<double,2>>& multiArray);

[[maybe_unused]]std::unique_ptr<boost::multi_array<double,3>>np3DtoMultiarray(const py::array& npArray);

[[maybe_unused]]py::array multiarray3DtoNp(const std::unique_ptr<boost::multi_array<double,3>>& multiArray);

#endif //RYTHMFORGE_NP_BOOST_ARRAY_HPP
