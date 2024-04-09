#ifndef RYTHMFORGE_LOAD_WAVE_HPP
#define RYTHMFORGE_LOAD_WAVE_HPP

#include <vector>
#include <map>
#include <iostream>
#include <fstream>


typedef unsigned char byte;

static std::map<std::string, uint8_t> waveFieldsMap = {
        {"chunkID", 4},
        {"chunkSize", 4},
        {"format", 4},          // In Python and in documentation this should be equal to 4, however in cpp it only works with length 3
        {"subchunk1ID", 3},
        {"subchunk1Size", 4},
        {"audioFormat", 2},
        {"numChannels", 2},
        {"sampleRate", 4},
        {"byteRate", 4},
        {"blockAlign", 2},
        {"bitsPerSample", 2},
        {"subchunk2ID", 4},
        {"subchunk2Size", 4}
};


struct WaveHeader{
    std::vector<byte> header;                    // When read properly always equal to 'RIFF'
    unsigned int chunkSize;             // Size of the file in bytes
    std::vector<byte> format;                     // When read properly always equal to 'WAVE'
    std::vector<byte> subchunk1ID;                // When read properly always equal to 'fmt '
    unsigned int subchunk1Size;         // Size of the rest of the Subchunk which follows this number
    uint16_t audioFormat;               // Usually PCM = 1 (Linear quantization). Other values indicate other forms of
                                        // compression
    uint16_t numChannels;               // Mono = 1, Stereo = 2, etc.
    unsigned int sampleRate;            // Typically 8000, 44100, etc.
    unsigned int byteRate;              // sampleRate * numChannels * bitsPerSample/8
    uint16_t blockAlign;                // numChannels * bitsPerSample / 8
    uint16_t bitsPerSample;
    // Other additional parameters

    std::vector<byte> subchunk2ID;                // When read properly always equal to 'data'
    unsigned int subchunk2Size;         // Size of the data in bytes, numSamples * numChannels * bitsPerSample / 8

    unsigned int data_start;
};


void load_wave(const std::ifstream& raw_data);
WaveHeader getFileProperties(std::ifstream& raw_data, unsigned int start_index);
std::vector<byte> readBytes(std::ifstream& raw_data, unsigned int data_length);
std::pair<std::vector<double>, unsigned int> readSamplesMono(std::ifstream& raw_data, WaveHeader& header);

#endif //RYTHMFORGE_LOAD_WAVE_HPP
