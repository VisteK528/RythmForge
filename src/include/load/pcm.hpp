#ifndef RYTHMFORGE_PCM_HPP
#define RYTHMFORGE_PCM_HPP

#include <vector>
#include <memory>
#include "../../third_party/multi_array.hpp"
#include "../../include/core/core.hpp"

using mdarray = boost::multi_array<double, 2>;
class PCMData{
private:
    // std::unique_ptr<std::vector<std::vector<double>>> samples_;
    std::unique_ptr<mdarray> samples_;
    unsigned int sampleRate_;
    uint16_t numChannels_;
public:
    PCMData(std::unique_ptr<mdarray> samples, unsigned int sampleRate);
    PCMData(const PCMData& other);

    double getDurationTime() const;
    const std::unique_ptr<mdarray>& getSamples() const;
    unsigned int getSampleRate() const;
    void toMono();
    // const std::pair<std::unique_ptr<std::vector<std::vector<byte>>>&, unsigned int> getSamplesAndSampleRate() const;
};

#endif //RYTHMFORGE_PCM_HPP
