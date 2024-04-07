#ifndef RYTHMFORGE_LOAD_WAVE_HPP
#define RYTHMFORGE_LOAD_WAVE_HPP

#include <vector>
#include <map>
#include <iostream>
#include <fstream>

typedef enum {
     chunkID = 4,
     chunkSize = 4,
     format = 4,
     subchunk1ID = 4,
     subchunk1Size = 2,
     numChannels = 2,
     sampleRate = 4,
     byteRate = 4,
     blockAlign = 2,
     bitsPerSample = 2,
     subchunk2ID = 4,
     subchunk2Size = 4
} waveFieldsLengths;



struct WaveHeader{
    char chunkID[4];                    // When read properly always equal to 'RIFF'
    unsigned int chunkSize;             // Size of the file in bytes
    char format[4];                     // When read properly always equal to 'WAVE'
    char subchunk1ID[4];                // When read properly always equal to 'fmt '
    unsigned int subchunk1Size;         // Size of the rest of the Subchunk which follows this number
    uint16_t audioFormat;               // Usually PCM = 1 (Linear quantization). Other values indicate other forms of
                                        // compression
    uint16_t numChannels;               // Mono = 1, Stereo = 2, etc.
    unsigned int sampleRate;            // Typically 8000, 44100, etc.
    unsigned int byteRate;              // sampleRate * numChannels * bitsPerSample/8
    uint16_t blockAlign;                // numChannels * bitsPerSample / 8

    // Other additional parameters

    char subchunk2ID[4];                // When read properly always equal to 'data'
    unsigned int subchunk2Size;         // Size of the data in bytes, numSamples * numChannels * bitsPerSample / 8
};


void load_wave(const std::ifstream& raw_data);
WaveHeader getFileProperties(std::ifstream& raw_data, unsigned int start_index);

#endif //RYTHMFORGE_LOAD_WAVE_HPP
