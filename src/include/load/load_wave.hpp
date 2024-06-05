#ifndef RYTHMFORGE_LOAD_WAVE_HPP
#define RYTHMFORGE_LOAD_WAVE_HPP

#include "../../include/core/core.hpp"
#include "pcm.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

namespace rythm_forge::audio::load {
    enum class WaveField : uint8_t {
        ChunkID = 4,
        ChunkSize = 4,
        Format = 4,
        Subchunk1ID = 3,
        Subchunk1Size = 4,
        AudioFormat = 2,
        NumChannels = 2,
        SampleRate = 4,
        ByteRate = 4,
        BlockAlign = 2,
        BitsPerSample = 2,
        Subchunk2ID = 4,
        Subchunk2Size = 4,
        DataStart = 43
    };


    struct WaveHeader {
        std::vector<byte> header;     // When read properly always equal to 'RIFF'
        unsigned int chunkSize;       // Size of the file in bytes
        std::vector<byte> format;     // When read properly always equal to 'WAVE'
        std::vector<byte> subchunk1ID;// When read properly always equal to 'fmt '
        unsigned int subchunk1Size;   // Size of the rest of the Subchunk which follows this number
        uint16_t audioFormat;         // Usually PCM = 1 (Linear quantization). Other values indicate other forms of
        // compression
        uint16_t numChannels;   // Mono = 1, Stereo = 2, etc.
        unsigned int sampleRate;// Typically 8000, 44100, etc.
        unsigned int byteRate;  // sampleRate * numChannels * bitsPerSample/8
        uint16_t blockAlign;    // numChannels * bitsPerSample / 8
        uint16_t bitsPerSample;
        // Other additional parameters

        std::vector<byte> subchunk2ID;// When read properly always equal to 'data'
        unsigned int subchunk2Size;   // Size of the data in bytes, numSamples * numChannels * bitsPerSample / 8
        unsigned int data_start;
    };


    class WaveLoader {
    private:
        static int readChannel(std::ifstream &raw_data, const short bytes_per_sample);
        std::vector<double> readSample(std::ifstream &raw_data, const short bytes_per_sample, const WaveHeader &header);

    public:
        std::optional<WaveHeader> getFileProperties(std::ifstream &file_handle);
        std::optional<PCMData> loadPCMData(std::ifstream &file_handle);
    };
}// namespace rythm_forge::audio::load

#endif//RYTHMFORGE_LOAD_WAVE_HPP
