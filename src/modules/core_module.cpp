#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <iostream>
#include "../include/core/fft.hpp"

namespace py = pybind11;

void SimpleTest(){
    std::cout<<"Hello Mr. Bean"<<std::endl;
}

py::array_t<rythm_forge::dcomplex> fft_python(py::array_t<rythm_forge::dcomplex>& input_samples){

    auto input = std::vector<rythm_forge::dcomplex>(input_samples.size());

    for (long i = 0; i < input_samples.size(); ++i) {
        input[i] = input_samples.at(i);
    }
    std::vector<rythm_forge::dcomplex> result = rythm_forge::fft::fft(input);
    py::array_t<rythm_forge::dcomplex> numpyArray(result.size(), result.data());
    return numpyArray;
}

py::array_t<rythm_forge::dcomplex> ifft_python(py::array_t<rythm_forge::dcomplex>& input_samples){

    auto input = std::vector<rythm_forge::dcomplex>(input_samples.size());

    for (long i = 0; i < input_samples.size(); ++i) {
        input[i] = input_samples.at(i);
    }
    std::vector<rythm_forge::dcomplex> result = rythm_forge::fft::ifft(input);
    py::array_t<rythm_forge::dcomplex> numpyArray(result.size(), result.data());
    return numpyArray;
}

PYBIND11_MODULE(rythm_forge_core_cpp, m) {
    m.doc() = "RythmForge core module";
    m.def("stft", &SimpleTest, "Short Time Fourier Transform");
    m.def("fft", &fft_python, "Fast Fourier Transform");
    m.def("ifft", &ifft_python, "Fast Fourier Transform");
}