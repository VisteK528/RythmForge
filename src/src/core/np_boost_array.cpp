#include "../../include/core/np_boost_array.hpp"
#include <complex>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

std::unique_ptr<boost::multi_array<double, 2>> np2DtoMultiarray(const py::array &npArray) {
    auto r = npArray.unchecked<double, 2>();
    std::vector<size_t> npArrayShape{};
    py::buffer_info buf_info = npArray.request();
    if (buf_info.ndim != 2) {
        throw std::runtime_error("Input dimensions do not match Boost MultiArray dimensions");
    }


    auto boost_array = std::make_unique<boost::multi_array<double, 2>>(boost::extents[buf_info.shape[1]][buf_info.shape[0]]);

    for (long i = 0; i < buf_info.shape[1]; ++i) {
        for (long j = 0; j < buf_info.shape[0]; ++j) {
            (*boost_array)[i][j] = r(j, i);
        }
    }
    return boost_array;
}

py::array_t<double> multiarray2DtoNp(const std::unique_ptr<boost::multi_array<double, 2>> &multiArray) {
    py::array_t<double> numpyArray({multiArray->shape()[1], multiArray->shape()[0]});
    auto npArrayData = numpyArray.mutable_unchecked<2>();
    for (size_t i = 0; i < multiArray->shape()[1]; ++i) {
        for (size_t j = 0; j < multiArray->shape()[0]; ++j) {
            npArrayData(i, j) = (*multiArray)[j][i];
        }
    }
    return numpyArray;
}

std::unique_ptr<boost::multi_array<double, 3>> np3DtoMultiarray(const py::array &npArray) {
    auto r = npArray.unchecked<double, 3>();
    std::vector<size_t> npArrayShape{};
    py::buffer_info buf_info = npArray.request();
    if (buf_info.ndim != 3) {
        throw std::runtime_error("Input dimensions do not match Boost MultiArray dimensions");
    }

    auto boost_array = std::make_unique<boost::multi_array<double, 3>>(boost::extents[buf_info.shape[0]][buf_info.shape[1]][buf_info.shape[2]]);

    for (int64_t i = 0; i < buf_info.shape[0]; ++i) {
        for (int64_t j = 0; j < buf_info.shape[1]; ++j) {
            for (int64_t k = 0; k < buf_info.shape[1]; ++k) {
                (*boost_array)[i][j][k] = r(i, j, k);
            }
        }
    }
    return boost_array;
}

py::array multiarray3DtoNp(const std::unique_ptr<boost::multi_array<double, 3>> &multiArray) {
    py::array_t<double> numpyArray({multiArray->shape()[0], multiArray->shape()[1], multiArray->shape()[2]});
    auto npArrayData = numpyArray.mutable_unchecked();
    for (size_t i = 0; i < multiArray->shape()[0]; ++i) {
        for (size_t j = 0; j < multiArray->shape()[1]; ++j) {
            for (size_t k = 0; k < multiArray->shape()[2]; ++k) {
                npArrayData(i, j, k) = (*multiArray)[i][j][k];
            }
        }
    }
    return numpyArray;
}


py::array multiarray3DtoNpComplex(const std::unique_ptr<boost::multi_array<rythm_forge::dcomplex , 3>> &multiArray) {
    py::array_t<rythm_forge::dcomplex> numpyArray({multiArray->shape()[0], multiArray->shape()[1], multiArray->shape()[2]});
    auto npArrayData = numpyArray.mutable_unchecked();
    for (size_t i = 0; i < multiArray->shape()[0]; ++i) {
        for (size_t j = 0; j < multiArray->shape()[1]; ++j) {
            for (size_t k = 0; k < multiArray->shape()[2]; ++k) {
                npArrayData(i, j, k) = (*multiArray)[i][j][k];
            }
        }
    }
    return numpyArray;
}

std::unique_ptr<boost::multi_array<rythm_forge::dcomplex , 3>> np3DtoMultiarrayComplex(const py::array &npArray) {
    auto r = npArray.unchecked<rythm_forge::dcomplex , 3>();
    std::vector<size_t> npArrayShape{};
    py::buffer_info buf_info = npArray.request();
    if (buf_info.ndim != 3) {
        throw std::runtime_error("Input dimensions do not match Boost MultiArray dimensions");
    }

    auto boost_array = std::make_unique<boost::multi_array<rythm_forge::dcomplex , 3>>(boost::extents[buf_info.shape[0]][buf_info.shape[1]][buf_info.shape[2]]);

    for (int64_t i = 0; i < buf_info.shape[0]; ++i) {
        for (int64_t j = 0; j < buf_info.shape[1]; ++j) {
            for (int64_t k = 0; k < buf_info.shape[1]; ++k) {
                (*boost_array)[i][j][k] = r(i, j, k);
            }
        }
    }
    return boost_array;
}
//void test_function(const py::array &npArray) {
//    auto a = np2multiarray2d<int, 2>(npArray);
//    auto b = np2multiarray2d<int, 3>(npArray);
//    auto c = np2multiarray2d<std::complex<double>, 3>(npArray);
//    auto d = np2multiarray2d<double, 3>(npArray);
//}
//
//void test_funcion2(boost::multi_array<double,2> array){
//    auto r = multiarray2np<double,2>(array);
//}