#include <numeric>
#include <cmath>
#include "../../include/load/load_wave.hpp"
#include "../../include/load/load.hpp"


std::optional<WaveHeader> WaveLoader::getFileProperties(std::ifstream &file_handle) {
    WaveHeader header;

    header.header = readBytes(file_handle, waveFieldsMap["chunkID"]);
    header.chunkSize = toLittleEndian(readBytes(file_handle, waveFieldsMap["chunkSize"]));
    header.format = readBytes(file_handle, waveFieldsMap["format"]);
    header.subchunk1ID = readBytes(file_handle, waveFieldsMap["subchunk1ID"]);
    header.subchunk1Size = toLittleEndian(readBytes(file_handle, waveFieldsMap["subchunk1Size"]));

    header.audioFormat = toLittleEndian(readBytes(file_handle, waveFieldsMap["audioFormat"]));
    header.numChannels = toLittleEndian(readBytes(file_handle, waveFieldsMap["numChannels"]));
    header.sampleRate = toLittleEndian(readBytes(file_handle, waveFieldsMap["sampleRate"]));
    header.byteRate = toLittleEndian(readBytes(file_handle, waveFieldsMap["byteRate"]));
    header.blockAlign = toLittleEndian(readBytes(file_handle, waveFieldsMap["blockAlign"]));
    header.bitsPerSample = toLittleEndian(readBytes(file_handle, waveFieldsMap["bitsPerSample"]));


    // Crawl to data chunk
    std::vector<byte> data_pattern = {0x64, 0x61, 0x74, 0x61};
    std::reverse(data_pattern.begin(), data_pattern.end());
    std::vector<byte> pattern = readBytes(file_handle, waveFieldsMap["subchunk2Size"]);

    header.data_start = 0;
    for (const auto& pair : waveFieldsMap) {
        header.data_start += pair.second;
    }

    while(1){
        if(pattern == data_pattern){
            header.subchunk2ID = data_pattern;
            break;
        }

        if(header.data_start >= header.chunkSize){
            return {};
        }

        pattern.pop_back();
        pattern.insert(pattern.begin(), readBytes(file_handle, 1)[0]);
        ++header.data_start;

    }
    header.subchunk2Size = toLittleEndian(readBytes(file_handle, waveFieldsMap["subchunk2Size"]));

    return header;
}

std::optional<PCMData> WaveLoader::loadPCMData(std::ifstream &file_handle) {
    auto optional_header = getFileProperties(file_handle);
    if(!optional_header.has_value()){
        return {};
    }
    WaveHeader header = optional_header.value();
    short bytes_per_sample = header.bitsPerSample / 8;

    std::unique_ptr<std::vector<std::vector<double>>> samples = std::make_unique<std::vector<std::vector<double>>>();
    samples->reserve(header.subchunk2Size / header.numChannels / bytes_per_sample);

    unsigned int data_size = header.subchunk2Size;
    int i = 0;
    while(data_size > 0){
        std::vector<double> sample_vec = readSample(file_handle, bytes_per_sample, header);
        auto divideBy = [](double val) { return val / 32767.; };

        std::transform(sample_vec.begin(), sample_vec.end(), sample_vec.begin(), divideBy);
        // double sample = (sample_vec[0] + sample_vec[1])/ static_cast<double>(sample_vec.size()) / 32767.;

        // std::cout<<sample<<std::endl;
        samples->push_back(sample_vec);
        data_size -= header.numChannels * bytes_per_sample;
        ++i;
    }

    return PCMData(std::move(samples), header.sampleRate);
}


WaveHeader getFileProperties(std::ifstream& raw_data){
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
        if(pattern == data_pattern){
            header.subchunk2ID = data_pattern;
            break;
        }
        pattern.pop_back();
        pattern.insert(pattern.begin(), readBytes(raw_data, 1)[0]);
        ++header.data_start;

    }
    header.subchunk2Size = toLittleEndian(readBytes(raw_data, waveFieldsMap["subchunk2Size"]));

    return header;
}


int WaveLoader::readChannel(std::ifstream &raw_data, const short bytes_per_sample) {
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

std::vector<double> WaveLoader::readSample(std::ifstream& raw_data, const short bytes_per_sample, const WaveHeader& header){
    std::vector<double> channels;
    channels.reserve(header.numChannels);
    for (int i = 0; i < header.numChannels; ++i) {
        channels.push_back(static_cast<double>(readChannel(raw_data, bytes_per_sample)));
    }
    return channels;
}