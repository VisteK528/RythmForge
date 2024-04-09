#include <iostream>
#include "include/load_wave.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::ifstream my_file;
    my_file.open("data/arcade.wav", std::ios::binary);

    if (!my_file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    WaveHeader my_header = getFileProperties(my_file, 0);

    std::pair<std::vector<double>, unsigned int> data = readSamplesMono(my_file, my_header);

    my_file.close();
    return 0;
}
