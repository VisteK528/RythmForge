#include "../../include/core/np_boost_array.hpp"


std::unique_ptr<boost::multi_array<double,2>>np2DtoMultiarray(const py::array& npArray){
    auto r = npArray.unchecked<double,2>();
    std::vector<size_t> npArrayShape{};
    py::buffer_info buf_info = npArray.request();
    if (buf_info.ndim != 2) {
        throw std::runtime_error("Input dimensions do not match Boost MultiArray dimensions");
    }

    auto boost_array = std::make_unique<boost::multi_array<double,2>>(boost::extents[buf_info.shape[0]][buf_info.shape[1]]);

    for(int64_t i =0;i<buf_info.shape[0];++i){
        for(int64_t j = 0; j<buf_info.shape[1];++j){
            boost_array->operator[](i)[j]=r(i,j);
        }
    }
    return boost_array;
}

std::unique_ptr<boost::multi_array<double,3>>np3DtoMultiarray(const py::array& npArray){
    auto r = npArray.unchecked<double,3>();
    std::vector<size_t> npArrayShape{};
    py::buffer_info buf_info = npArray.request();
    if (buf_info.ndim != 3) {
        throw std::runtime_error("Input dimensions do not match Boost MultiArray dimensions");
    }

    auto boost_array = std::make_unique<boost::multi_array<double,3>>(boost::extents[buf_info.shape[0]][buf_info.shape[1]][buf_info.shape[2]]);

    for(int64_t i =0;i<buf_info.shape[0];++i){
        for(int64_t j = 0; j<buf_info.shape[1];++j){
            for(int64_t k = 0; k<buf_info.shape[1];++k){
                boost_array->operator[](i)[j][k]=r(i,j,k);
            }
        }
    }
    return boost_array;
}

static py::array multiarray2DtoNp(const std::unique_ptr<boost::multi_array<double,2>>& multiArray){
    py::array_t<double> numpyArray( {multiArray->shape()[0],multiArray->shape()[1]});
    auto npArrayData = numpyArray.mutable_unchecked();
    for(size_t i = 0; i < multiArray->shape()[0]; ++i){
        for(size_t j = 0; j < multiArray->shape()[1]; ++j){
            npArrayData(i,j) = multiArray->operator[](i)[j];
        }
    }
    return numpyArray;

}

static py::array multiarray3DtoNp(const std::unique_ptr<boost::multi_array<double,3>>& multiArray){
    py::array_t<double> numpyArray( {multiArray->shape()[0],multiArray->shape()[1],multiArray->shape()[2]});
    auto npArrayData = numpyArray.mutable_unchecked();
    for(size_t i = 0; i < multiArray->shape()[0]; ++i){
        for(size_t j = 0; j < multiArray->shape()[1]; ++j){
            for (size_t k = 0; k < multiArray->shape()[2]; ++k) {
                npArrayData(i,j,k) = multiArray->operator[](i)[j][k];
            }
        }
    }
    return numpyArray;

}