#include "../../include/load/load.hpp"

unsigned int toLittleEndian(const std::vector<byte>& bytes){
    unsigned int value = 0;

    unsigned int i = 0;
    for(const byte& var: bytes){
        value |= static_cast<uint8_t>(var) << i * 8;
        ++i;
    }
    return value;
}

std::vector<byte> readBytes(std::ifstream& raw_data, unsigned int data_length){
    std::vector<byte> bytes;
    bytes.reserve(data_length);

    for(auto i = 0; i < data_length; ++i){
        byte value = 0;
        raw_data >> value;
        bytes.push_back(value);
    }

    return bytes;
}