#include <pybind11/pybind11.h>
#include <iostream>

void SimpleTest(){
    std::cout<<"Hello Mr. Bean"<<std::endl;
}

PYBIND11_MODULE(rythm_forge_core_cpp, m) {
    m.doc() = "RythmForge core module";
    m.def("stft", &SimpleTest, "Short Time Fourier Transform");
}