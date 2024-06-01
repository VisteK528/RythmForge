#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <iostream>
#include "../include/core/fft.hpp"
#include "../include/load/pcm.hpp"

namespace py = pybind11;


static py::array_t<rythm_forge::dcomplex> assignNumpyArray(const std::unique_ptr<rythm_forge::c3array>& samples){
    const size_t numChannels = samples->shape()[0];
    const size_t frequencyBins = samples->shape()[1];
    const size_t frames = samples->shape()[2];

    py::array_t<rythm_forge::dcomplex> numpyArray({numChannels, frequencyBins, frames});

    auto numpyArrayData = numpyArray.mutable_unchecked<3>();
    for(size_t i = 0; i < numChannels; ++i){
        for(size_t j = 0; j < frequencyBins; ++j){
            for(size_t k = 0; k < frames; ++k){
                numpyArrayData(i, j, k) = (*samples)[i][j][k];
            }

        }
    }
    return numpyArray;
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

py::array_t<rythm_forge::dcomplex> stft_python(py::array_t<double>& input_samples){
    auto r = input_samples.unchecked<2>();
    size_t channels = r.shape(0);
    size_t samples_len = r.shape(1);

    auto result = std::make_unique<rythm_forge::d2array>(boost::extents[samples_len][channels]);

    for (size_t i = 0; i < samples_len; ++i) {
        for (size_t j = 0; j < channels; ++j) {
            (*result)[i][j] = r(j, i);
        }
    }

    rythm_forge::PCMData data(std::move(result), 44100);
    std::unique_ptr<rythm_forge::c3array> stft_matrix = rythm_forge::fft::stft(data.getSamples(), 2048,
                                                                               512, 2048,
                                                                               false);


    py::array_t<rythm_forge::dcomplex> numpyArray = assignNumpyArray(stft_matrix);
    return numpyArray;
}


PYBIND11_MODULE(rythm_forge_core_cpp, m) {
    m.doc() = "RythmForge core module";
    m.def("stft", &stft_python, "Short Time Fourier Transform");
    m.def("fft", &fft_python, "Fast Fourier Transform");
    m.def("ifft", &ifft_python, "Fast Fourier Transform");
}