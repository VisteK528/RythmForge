#ifndef RYTHMFORGE_PCM_HPP
#define RYTHMFORGE_PCM_HPP

#include <vector>
#include <memory>
#include <boost/multi_array.hpp>
#include "../../include/core/core.hpp"

namespace rythm_forge{
    class PCMData{
    private:
        // std::unique_ptr<std::vector<std::vector<double>>> samples_;
        std::unique_ptr<d2array> samples_;
        unsigned int sampleRate_;
        uint16_t numChannels_;
    public:
        PCMData(std::unique_ptr<d2array> samples, unsigned int sampleRate);
        PCMData(const PCMData& other);

        double getDurationTime() const;
        const std::unique_ptr<d2array>& getSamples() const;
        unsigned int getSampleRate() const;
        void toMono();
        // const std::pair<std::unique_ptr<std::vector<std::vector<byte>>>&, unsigned int> getSamplesAndSampleRate() const;
    };
}

#endif //RYTHMFORGE_PCM_HPP
