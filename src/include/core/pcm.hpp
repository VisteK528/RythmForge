#ifndef RYTHMFORGE_PCM_HPP
#define RYTHMFORGE_PCM_HPP

#include <vector>
#include <memory>
#include "core.hpp"


class PCMData{
private:
    std::unique_ptr<std::vector<std::vector<double>>> samples_;
    unsigned int sampleRate_;
public:
    PCMData(std::unique_ptr<std::vector<std::vector<double>>> samples, unsigned int sampleRate): samples_(std::move(samples)), sampleRate_(sampleRate) {}
    PCMData(const PCMData& other);

    double getDurationTime() const;
    const std::unique_ptr<std::vector<std::vector<double>>>& getSamples() const;
    unsigned int getSampleRate() const;
    // const std::pair<std::unique_ptr<std::vector<std::vector<byte>>>&, unsigned int> getSamplesAndSampleRate() const;
};

#endif //RYTHMFORGE_PCM_HPP
