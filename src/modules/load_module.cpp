#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../include/load/load_wave.hpp"

namespace py = pybind11;

static py::array_t<double> assignNumpyArray(const std::unique_ptr<rythm_forge::d2array>& samples, size_t numChannels){
    py::array_t<double> numpyArray({numChannels, samples->size()});

    auto numpyArrayData = numpyArray.mutable_unchecked<2>();
    for(size_t i = 0; i < numChannels; ++i){
        for(size_t j = 0; j < samples->size(); ++j){
            numpyArrayData(i, j) = (*samples)[j][i];
        }
    }
    return numpyArray;
}


py::tuple readPCMData(py::object& raw_data) {
    std::ifstream file;

    file.open(py::str(raw_data), std::ios::binary);

    rythm_forge::audio::load::WaveLoader loader;
    auto optional_data = loader.loadPCMData(file);
    if(!optional_data.has_value()){
        throw std::runtime_error("An error has occured!");
    }

    rythm_forge::PCMData data = optional_data.value();
    file.close();

    const std::unique_ptr<rythm_forge::d2array>& samples = data.getSamples();
    unsigned int sampleRate = data.getSampleRate();
    size_t numChannels = samples->shape()[1];

    // Constructing NumPy arrays
    py::array_t<double> numpyArray = assignNumpyArray(samples, numChannels);
    return py::make_tuple(numpyArray, sampleRate);
}

py::array_t<double> converToMono(py::array_t<double>& input_samples){
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
    data.toMono();

    const std::unique_ptr<rythm_forge::d2array>& samples = data.getSamples();
    size_t numChannels = samples->shape()[1];
    return assignNumpyArray(samples, numChannels);
}


// Define Python module
PYBIND11_MODULE(rythm_forge_load_cpp, m) {
    m.doc() = "RythmForge audio files loading module";
    m.def("load_wav_file", &readPCMData, "Reads samples from .wav file and returns them with sample rate");
    m.def("to_mono", &converToMono);
}