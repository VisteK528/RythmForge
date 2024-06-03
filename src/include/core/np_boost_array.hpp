//
// Created by damia on 03.06.2024.
//

#ifndef RYTHMFORGE_NP_BOOST_ARRAY_HPP
#define RYTHMFORGE_NP_BOOST_ARRAY_HPP
#include <pybind11/numpy.h>
#include <boost/multi_array.hpp>
#include <memory>
namespace py = pybind11;
template<typename T,uint8_t N>
std::unique_ptr<boost::multi_array<T,N>> np2multiarray(const py::array& npArray){
    std::vector<size_t> npArrayShape{};
    py::buffer_info buf_info = npArray.request();
    if (buf_info.ndim != N) {
        throw std::runtime_error("Input dimensions do not match Boost MultiArray dimensions");
    }
    std::array<std::size_t, N> shape;
    for (std::size_t i = 0; i < N; ++i) {
        shape[i] = buf_info.shape[i];
    }
//TODO check
    auto boost_array = std::make_unique<boost::multi_array<T,N>>(shape);
    T* data_ptr = static_cast<T*>(buf_info.ptr);
//TODO    Check data copying if fails its doomed
//    std::copy(data_ptr, data_ptr + buf_info.size, boost_array.data());
    return boost_array;
}
//TODO maybe its better to just deref pointer
template<typename T, std::size_t N>
py::array multiarray2np(const std::unique_ptr<boost::multi_array<T, N>>& boost_array) {
    // Extract shape information from the Boost MultiArray
    std::vector<ssize_t> shape(boost_array.shape(), boost_array.shape() + N);

    // Create a new NumPy array with the same shape
    py::array_t<T> result(shape);

    // Request a buffer descriptor from the NumPy array
    py::buffer_info result_buf_info = result.request();

    // Copy data from the Boost MultiArray to the NumPy array
    T* result_data_ptr = static_cast<T*>(result_buf_info.ptr);
    std::copy(boost_array.data(), boost_array.data() + boost_array.num_elements(), result_data_ptr);

    return result;
}

#endif //RYTHMFORGE_NP_BOOST_ARRAY_HPP
