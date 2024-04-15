#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "../include/load_wave.hpp"

namespace py = pybind11;


// Function to wrap readBytes and return result as NumPy array
py::tuple readBytesAsNumPy(py::object raw_data) {
    // Convert Python file object to C++ ifstream
    std::ifstream file;

    file.open(py::str(raw_data), std::ios::binary);

    // file.open("../data/arcade.wav", std::ios::binary);

    // Call readBytes function

    WaveHeader my_header = getFileProperties(file, 0);
    std::pair<std::vector<double>, unsigned int> data = readSamplesMono(file, my_header);

    // Close the file
    file.close();

    // Create NumPy array from the std::vector<byte>
    py::array_t<double> samples_array(data.first.size(), data.first.data());
    return py::make_tuple(samples_array, data.second);
}


// Define Python module
PYBIND11_MODULE(rythm_forge_load_cpp, m) {
    m.doc() = "This is a simple doc";
    m.def("read_bytes", &readBytesAsNumPy, "Read bytes from file and return as NumPy array");
}