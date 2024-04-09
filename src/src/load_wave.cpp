#include <numeric>
#include "../include/load_wave.hpp"


void load_wave(const std::ifstream& raw_data){
    unsigned int start_index = 0;
    const uint8_t HEADER_SIZE = 4;

    getFileProperties(const_cast<std::ifstream&>(raw_data), start_index);
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

unsigned int toLittleEndian(const std::vector<byte>& bytes){
    unsigned int value = 0;

    unsigned int i = 0;
    for(const byte& var: bytes){
        value |= static_cast<uint8_t>(var) << i * 8;
        ++i;
    }
    return value;
}


WaveHeader getFileProperties(std::ifstream& raw_data, unsigned int start_index){
    WaveHeader header;

    header.header = readBytes(raw_data, waveFieldsMap["chunkID"]);
    header.chunkSize = toLittleEndian(readBytes(raw_data, waveFieldsMap["chunkSize"]));
    header.format = readBytes(raw_data, waveFieldsMap["format"]);
    header.subchunk1ID = readBytes(raw_data, waveFieldsMap["subchunk1ID"]);
    header.subchunk1Size = toLittleEndian(readBytes(raw_data, waveFieldsMap["subchunk1Size"]));

    header.audioFormat = toLittleEndian(readBytes(raw_data, waveFieldsMap["audioFormat"]));
    header.numChannels = toLittleEndian(readBytes(raw_data, waveFieldsMap["numChannels"]));
    header.sampleRate = toLittleEndian(readBytes(raw_data, waveFieldsMap["sampleRate"]));
    header.byteRate = toLittleEndian(readBytes(raw_data, waveFieldsMap["byteRate"]));
    header.blockAlign = toLittleEndian(readBytes(raw_data, waveFieldsMap["blockAlign"]));
    header.bitsPerSample = toLittleEndian(readBytes(raw_data, waveFieldsMap["bitsPerSample"]));


    // Crawl to data chunk
    std::vector<byte> data_pattern = {0x64, 0x61, 0x74, 0x61};
    std::reverse(data_pattern.begin(), data_pattern.end());
    std::vector<byte> pattern = readBytes(raw_data, waveFieldsMap["subchunk2Size"]);

    header.data_start = 0;
    for (const auto& pair : waveFieldsMap) {
        header.data_start += pair.second;
    }

    while(1){
        pattern.pop_back();
        pattern.insert(pattern.begin(), readBytes(raw_data, 1)[0]);
        if(pattern == data_pattern){
            header.subchunk2ID = data_pattern;
            break;
        }
        ++header.data_start;
    }
    header.subchunk2Size = toLittleEndian(readBytes(raw_data, waveFieldsMap["subchunk2Size"]));

    return header;
}


int readChannel(std::ifstream& raw_data, const short bytes_per_sample){
    char data[bytes_per_sample];
    raw_data.read(data, bytes_per_sample);

    uint16_t raw_value = static_cast<unsigned char>(data[0]) | static_cast<unsigned char>(data[1]) << 8;

    // Convert to Two's complement
    int value = static_cast<int>(raw_value);
    if ((raw_value & (1 << (8 * bytes_per_sample - 1))) != 0) {
        value -= (1 << (8 * bytes_per_sample));
    }

    return value;
}

std::vector<double> readSample(std::ifstream& raw_data, const short bytes_per_sample, WaveHeader& header){
    std::vector<double> channels;
    channels.reserve(header.numChannels);
    for (int i = 0; i < header.numChannels; ++i) {
        channels.push_back(static_cast<double>(readChannel(raw_data, bytes_per_sample)));
    }
    return channels;
}

std::pair<std::vector<double>, unsigned int> readSamplesMono(std::ifstream& raw_data, WaveHeader& header){
    short bytes_per_sample = header.bitsPerSample / 8;

    std::vector<double> samples;
    samples.reserve(header.subchunk2Size / header.numChannels / bytes_per_sample);

    unsigned int data_size = header.subchunk2Size;
    int i = 0;
    while(data_size > 0){
        std::vector<double> sample_vec = readSample(raw_data, bytes_per_sample, header);
        double sample = (sample_vec[0] + sample_vec[1])/ static_cast<double>(sample_vec.size()) / 32767.;

        // std::cout<<sample<<std::endl;
        samples.push_back(sample);
        data_size -= header.numChannels * bytes_per_sample;
        ++i;
    }
    return std::pair(samples, header.sampleRate);
}
