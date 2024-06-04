
#ifndef RYTHMFORGE_BEAT_HPP
#define RYTHMFORGE_BEAT_HPP
#include <vector>
#include "../../include/load/pcm.hpp"
#include "fft.hpp"
#include "core.hpp"
namespace rythm_forge::beat {
    std::vector<double> beat(const rythm_forge::PCMData &pcm);
    std::unique_ptr<d2array > create_mel_filter_bank(uint32_t sampleRate,uint32_t nFft,uint32_t nMels);

}
#endif //RYTHMFORGE_BEAT_HPP
