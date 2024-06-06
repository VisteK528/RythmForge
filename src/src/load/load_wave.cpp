#include "../../include/load/load_wave.hpp"
#include "../../include/load/load.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>


namespace rythm_forge::audio::load {
    std::optional<WaveHeader> WaveLoader::getFileProperties(std::ifstream &file_handle) {

        WaveHeader header;

        header.header = readBytes(file_handle, static_cast<unsigned int>(WaveField::ChunkID));
        header.chunkSize = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::ChunkSize)));
        header.format = readBytes(file_handle, static_cast<unsigned int>(WaveField::Format));
        header.subchunk1ID = readBytes(file_handle, static_cast<unsigned int>(WaveField::Subchunk1ID));
        header.subchunk1Size = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::Subchunk1Size)));

        header.audioFormat = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::AudioFormat)));
        header.numChannels = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::NumChannels)));
        header.sampleRate = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::SampleRate)));
        header.byteRate = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::ByteRate)));
        header.blockAlign = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::BlockAlign)));
        header.bitsPerSample = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::BitsPerSample)));


        // Crawl to data chunk
        std::vector<byte> data_pattern = {0x64, 0x61, 0x74, 0x61};
        std::reverse(data_pattern.begin(), data_pattern.end());
        std::vector<byte> pattern = readBytes(file_handle, static_cast<unsigned int>(WaveField::Subchunk2Size));

        header.data_start = static_cast<uint32_t>(WaveField::DataStart);

        while (1) {
            if (pattern == data_pattern) {
                header.subchunk2ID = data_pattern;
                break;
            }

            if (header.data_start >= header.chunkSize) {
                std::cout << header.data_start << " i " << header.chunkSize;
                return {};
            }

            pattern.pop_back();
            pattern.insert(pattern.begin(), readBytes(file_handle, 1)[0]);
            ++header.data_start;
        }
        header.subchunk2Size = toLittleEndian(readBytes(file_handle, static_cast<unsigned int>(WaveField::Subchunk2Size)));

        return header;
    }

    std::optional<PCMData> WaveLoader::loadPCMData(std::ifstream &file_handle) {

        auto optional_header = getFileProperties(file_handle);
        if (!optional_header.has_value()) {

            return {};
        }
        WaveHeader header = optional_header.value();
        short bytes_per_sample = header.bitsPerSample / 8;

        std::unique_ptr<d2array> samples = std::make_unique<d2array>(boost::extents[header.subchunk2Size / header.numChannels / bytes_per_sample][header.numChannels]);


        unsigned int data_size = header.subchunk2Size;
        int i = 0;
        while (data_size > 0) {
            std::vector<double> sample_vec = readSample(file_handle, bytes_per_sample, header);
            auto divideBy = [](double val) { return val / 32767.; };

            std::transform(sample_vec.begin(), sample_vec.end(), sample_vec.begin(), divideBy);


            std::copy(sample_vec.cbegin(), sample_vec.cend(), (*samples)[i].begin());
            data_size -= header.numChannels * bytes_per_sample;
            ++i;
        }

        return PCMData(std::move(samples), header.sampleRate);
    }


    int WaveLoader::readChannel(std::ifstream &raw_data, const short bytes_per_sample) {
        std::unique_ptr<char[]> data(new char[bytes_per_sample]);
        for (auto i = 0; i < bytes_per_sample; i++) {
            char read_byte = 0;
            raw_data.read(&read_byte, 1);
            data[i] = read_byte;
        }

        uint16_t raw_value = static_cast<unsigned char>(data[0]) | static_cast<unsigned char>(data[1]) << 8;

        // Convert to Two's complement
        int value = static_cast<int>(raw_value);
        if ((raw_value & (1 << (8 * bytes_per_sample - 1))) != 0) {
            value -= (1 << (8 * bytes_per_sample));
        }

        return value;
    }

    std::vector<double> WaveLoader::readSample(std::ifstream &raw_data, const short bytes_per_sample, const WaveHeader &header) {
        std::vector<double> channels;
        channels.reserve(header.numChannels);
        for (int i = 0; i < header.numChannels; ++i) {
            channels.push_back(static_cast<double>(readChannel(raw_data, bytes_per_sample)));
        }
        return channels;
    }
}// namespace rythm_forge::audio::load