#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../include/load/load_wave.hpp"

namespace py = pybind11;


py::tuple readBytesAsNumPy(py::object raw_data) {
    std::ifstream file;

    file.open(py::str(raw_data), std::ios::binary);

    WaveLoader loader;
    auto optional_data = loader.loadPCMData(file);
    if(!optional_data.has_value()){
        throw std::runtime_error("An error has occured!");
    }

    PCMData data = optional_data.value();
    file.close();

    const std::unique_ptr<std::vector<std::vector<double>>>& samples = data.getSamples();
    unsigned int sampleRate = data.getSampleRate();
    size_t numChannels = samples->at(0).size();

    // Constructing NumPy arrays
    py::array_t<double> numpyArray({numChannels, samples->size()});
    auto numpyArrayData = numpyArray.mutable_unchecked<2>();
    for(size_t i = 0; i < numChannels; ++i){
        for(size_t j = 0; j < samples->size(); ++j){
            numpyArrayData(i, j) = samples->at(j)[i];
        }
    }

    return py::make_tuple(numpyArray, sampleRate);
}


// Define Python module
PYBIND11_MODULE(rythm_forge_load_cpp, m) {
    m.doc() = "RythmForge audio files loading module";
    m.def("loadWavFile", &readBytesAsNumPy, "Reads samples from .wav file and returns them with sample rate");
}