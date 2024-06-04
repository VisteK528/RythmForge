#ifndef RYTHMFORGE_NP_BOOST_ARRAY_HPP
#define RYTHMFORGE_NP_BOOST_ARRAY_HPP
#include "pybind11/pybind11.h"
#include <boost/multi_array.hpp>
#include <memory>
#include <pybind11/numpy.h>
namespace py = pybind11;

std::unique_ptr<boost::multi_array<double, 2>> np2DtoMultiarray(const py::array &npArray);
py::array_t<double> multiarray2DtoNp(const std::unique_ptr<boost::multi_array<double, 2>> &multiArray);

[[maybe_unused]] std::unique_ptr<boost::multi_array<double, 3>> np3DtoMultiarray(const py::array &npArray);

[[maybe_unused]] py::array multiarray3DtoNp(const std::unique_ptr<boost::multi_array<double, 3>> &multiArray);


template<typename T, size_t N>
std::unique_ptr<boost::multi_array<T, N>> np2multiarray2d(const py::array &npArray) {
    //    auto r = npArray.unchecked<T,2>();
    //
    //    py::buffer_info bufferInfo = npArray.request();
    //    if (bufferInfo.ndim != 2) {
    //        throw std::runtime_error("Input dimensions do not match Boost MultiArray dimensions");
    //    }

    py::buffer_info bufInfo = npArray.request();


    std::vector<std::size_t> shape(bufInfo.shape.begin(), bufInfo.shape.end());
    boost::array<typename boost::multi_array<T, N>::index, N> extents;
    std::copy(shape.begin(), shape.end(), extents.begin());

    auto boostArray = std::make_unique<boost::multi_array<T, N>>(extents);


    T *pyData = static_cast<T *>(bufInfo.ptr);
    std::copy(pyData, pyData + bufInfo.size, boostArray->origin());

    return boostArray;
}

template<typename T, std::size_t N>
py::array multiarray2np(std::unique_ptr<boost::multi_array<T, N>> &boostArray) {


    std::vector<size_t> shape(boostArray->shape(), boostArray->shape() + N);
    std::vector<size_t> strides(N);
    for (std::size_t i = 0; i < N; ++i) {
        strides[i] = boostArray->strides()[i] * sizeof(T);
    }


    std::vector<T> data(boostArray->data(), boostArray->data() + boostArray->num_elements());

    return py::array(py::buffer_info(
            data.data(),
            sizeof(T),
            py::format_descriptor<T>::format(),
            N,
            shape,
            strides
            ));
}
#endif//RYTHMFORGE_NP_BOOST_ARRAY_HPP
