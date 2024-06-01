#ifndef RYTHMFORGE_LOAD_HPP
#define RYTHMFORGE_LOAD_HPP

#include <vector>
#include <fstream>
#include "../core/core.hpp"

namespace rythm_forge::audio::load{
    std::vector<byte> readBytes(std::ifstream& raw_data, unsigned int data_length);
    unsigned int toLittleEndian(const std::vector<byte>& bytes);
}


#endif //RYTHMFORGE_LOAD_HPP
