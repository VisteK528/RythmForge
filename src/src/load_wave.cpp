#include "../include/load_wave.hpp"


void load_wave(const std::ifstream& raw_data){
    unsigned int start_index = 0;
    const uint8_t HEADER_SIZE = 4;

    getFileProperties(const_cast<std::ifstream&>(raw_data), start_index);
}

WaveHeader getFileProperties(std::ifstream& raw_data, unsigned int start_index){
    char header_string[4];
    raw_data.read(header_string, 4);

    std::cout<< header_string << std::endl;

    volatile short count = raw_data.gcount();


    WaveHeader header{
        .chunkID = {'R', 'I', 'F', 'F'},
        .chunkSize = 0,
        .format = {'W', 'A', 'V', 'E'},
        .subchunk1ID = {'f', 'm', 't', ' '},
        .subchunk1Size = 0,
        .audioFormat = 0,
        .numChannels = 0,
        .sampleRate = 0,
        .byteRate = 0,
        .blockAlign = 0,
        .subchunk2ID = {'d', 'a', 't', 'a'},
        .subchunk2Size = 0

    };
    return header;
}
