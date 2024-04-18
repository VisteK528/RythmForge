#include <iostream>
#include "include/load/load_wave.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;


    std::ifstream my_file;
    my_file.open("data/waves.wav", std::ios::binary);

    if (!my_file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    WaveLoader my_loader;
    auto optional_header = my_loader.getFileProperties(my_file);
    if(optional_header.has_value()){
        WaveHeader my_header = optional_header.value();
    }

    // std::pair<std::vector<double>, unsigned int> data = readSamplesMono(my_file, my_header);

    my_file.close();
    return 0;
}
