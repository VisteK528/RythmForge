#include "../include/core/beat.hpp"
#include "../include/core/fft.hpp"
#include "../include/core/np_boost_array.hpp"
#include "../include/load/pcm.hpp"
#include <iostream>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

static py::array_t<rythm_forge::dcomplex> assignNumpyArray(const std::unique_ptr<rythm_forge::c3array> &samples) {
    const size_t numChannels = samples->shape()[0];
    const size_t frequencyBins = samples->shape()[1];
    const size_t frames = samples->shape()[2];

    py::array_t<rythm_forge::dcomplex> numpyArray({numChannels, frequencyBins, frames});

    auto numpyArrayData = numpyArray.mutable_unchecked<3>();
    for (size_t i = 0; i < numChannels; ++i) {
        for (size_t j = 0; j < frequencyBins; ++j) {
            for (size_t k = 0; k < frames; ++k) {
                numpyArrayData(i, j, k) = (*samples)[i][j][k];
            }
        }
    }
    return numpyArray;
}

[[maybe_unused]] static py::array_t<double> assignNumpyArray(const std::unique_ptr<rythm_forge::d3array> &samples) {
    const size_t numChannels = samples->shape()[0];
    const size_t frequencyBins = samples->shape()[1];
    const size_t frames = samples->shape()[2];

    py::array_t<rythm_forge::dcomplex> numpyArray({numChannels, frequencyBins, frames});

    auto numpyArrayData = numpyArray.mutable_unchecked<3>();
    for (size_t i = 0; i < numChannels; ++i) {
        for (size_t j = 0; j < frequencyBins; ++j) {
            for (size_t k = 0; k < frames; ++k) {
                numpyArrayData(i, j, k) = (*samples)[i][j][k];
            }
        }
    }
    return numpyArray;
}
static py::array_t<double> assignNumpyArrayDouble(const std::unique_ptr<rythm_forge::d2array> &samples, size_t numChannels) {
    py::array_t<double> numpyArray({numChannels, samples->size()});

    auto numpyArrayData = numpyArray.mutable_unchecked<2>();
    for (size_t i = 0; i < numChannels; ++i) {
        for (size_t j = 0; j < samples->size(); ++j) {
            numpyArrayData(i, j) = (*samples)[j][i];
        }
    }
    return numpyArray;
}

py::array_t<rythm_forge::dcomplex> fft_python(py::array_t<rythm_forge::dcomplex> &input_samples) {

    auto input = std::vector<rythm_forge::dcomplex>(input_samples.size());

    for (long i = 0; i < input_samples.size(); ++i) {
        input[i] = input_samples.at(i);
    }
    std::vector<rythm_forge::dcomplex> result = rythm_forge::fft::fft(input);
    py::array_t<rythm_forge::dcomplex> numpyArray(result.size(), result.data());
    return numpyArray;
}

py::array_t<rythm_forge::dcomplex> ifft_python(py::array_t<rythm_forge::dcomplex> &input_samples) {

    auto input = std::vector<rythm_forge::dcomplex>(input_samples.size());

    for (long i = 0; i < input_samples.size(); ++i) {
        input[i] = input_samples.at(i);
    }
    std::vector<rythm_forge::dcomplex> result = rythm_forge::fft::ifft(input);
    py::array_t<rythm_forge::dcomplex> numpyArray(result.size(), result.data());
    return numpyArray;
}

py::array_t<rythm_forge::dcomplex> stft_python(py::array_t<double> &input_samples, const int nFft, const int hopSize, const int windowLength, bool center) {
    auto r = input_samples.unchecked<2>();
    size_t channels = r.shape(0);
    size_t samples_len = r.shape(1);

    auto result = std::make_unique<rythm_forge::d2array>(boost::extents[samples_len][channels]);

    for (size_t i = 0; i < samples_len; ++i) {
        for (size_t j = 0; j < channels; ++j) {
            (*result)[i][j] = r(j, i);
        }
    }

    std::unique_ptr<rythm_forge::c3array> stft_matrix = rythm_forge::fft::stft(result, nFft,
                                                                               hopSize, windowLength,
                                                                               center);
    py::array_t<rythm_forge::dcomplex> numpyArray = assignNumpyArray(stft_matrix);
    return numpyArray;
}

py::array_t<double> istft_python(py::array_t<rythm_forge::dcomplex> &input_samples, const int nFft, const int hopSize, const int windowLength, bool center) {
    auto r = input_samples.unchecked<3>();
    size_t channels = r.shape(0);
    size_t frequencyBins = r.shape(1);
    size_t frames = r.shape(2);

    auto result = std::make_unique<rythm_forge::c3array>(boost::extents[channels][frequencyBins][frames]);
    for (size_t i = 0; i < channels; ++i) {
        for (size_t j = 0; j < frequencyBins; ++j) {
            for (size_t k = 0; k < frames; ++k) {
                (*result)[i][j][k] = r(i, j, k);
            }
        }
    }

    std::unique_ptr<rythm_forge::d2array> istftResult = rythm_forge::fft::istft(result, nFft,
                                                                                hopSize, windowLength,
                                                                                center);

    py::array_t<double> numpyArray = assignNumpyArrayDouble(istftResult, channels);
    return numpyArray;
}

py::tuple resample_python(py::array_t<double> &inputSample, int sr, int newSr) {
    py::buffer_info buf_info = inputSample.request();
    auto dataArray = np2DtoMultiarray(inputSample);
    rythm_forge::PCMData data(std::move(dataArray), sr);
    auto newPCM = rythm_forge::PCMData::resample(data, newSr);

    py::array_t<double> numpyArray = multiarray2DtoNp(newPCM.getSamples());
    return py::make_tuple(numpyArray, newSr);
}

py::array_t<double> create_mel_filter_bank_matrix(int sampleRate, int nFft, int nMels) {
    std::unique_ptr<rythm_forge::d2array> mel_filter_bank_matrix = rythm_forge::beat::create_mel_filter_bank(sampleRate, nFft, nMels);
    return multiarray2np(*mel_filter_bank_matrix);
}


PYBIND11_MODULE(rythm_forge_core_cpp, m) {
    m.doc() = "RythmForge core module";
    m.def("stft", &stft_python, "Short Time Fourier Transform");
    m.def("istft", &istft_python, "Inverse Short Time Fourier Transform");
    m.def("fft", &fft_python, "Fast Fourier Transform");
    m.def("ifft", &ifft_python, "Fast Fourier Transform");
    m.def("resample", &resample_python, "Resample");
    m.def("mel_filter_bank", &create_mel_filter_bank_matrix, "Mel filter bank");
    // m.def("magnitude",&getMagnitude,"Calculate magnitude of a complex-valued matrix");
}