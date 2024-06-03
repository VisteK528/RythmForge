
#ifndef RYTHMFORGE_BEAT_HPP
#define RYTHMFORGE_BEAT_HPP
#include <vector>
#include "../../include/load/pcm.hpp"
#include "fft.hpp"
#include "core.hpp"
namespace rythm_forge::beat {
    std::vector<double> beat(const rythm_forge::PCMData &pcm);
    std::unique_ptr<d2array > create_mel_filter_bank(PCMData& pcmData,int n_fft,int n_mels);
    std::unique_ptr<d3array >apply_mel_filter_bank(std::unique_ptr<d3array> &magnitudeMatrix, std::unique_ptr<d2array > &mel_filter_bank);
    void tempo(const rythm_forge::PCMData &pcm);

}
#endif //RYTHMFORGE_BEAT_HPP
