#ifndef RYTHMFORGE_PCM_HPP
#define RYTHMFORGE_PCM_HPP

#include "../../include/core/core.hpp"
#include <boost/multi_array.hpp>
#include <memory>
#include <vector>

namespace rythm_forge {
    class PCMData {
    private:
        std::unique_ptr<d2array> samples_;
        unsigned int sampleRate_;
        uint16_t numChannels_;

    public:
        PCMData(std::unique_ptr<d2array> samples, unsigned int sampleRate);
        PCMData(const PCMData &other);
        static PCMData resample(const PCMData &other, unsigned int target_sr);
        double getDurationTime() const;
        const std::unique_ptr<d2array> &getSamples() const;
        unsigned int getSampleRate() const;
        void toMono();
    };
}// namespace rythm_forge

#endif//RYTHMFORGE_PCM_HPP
