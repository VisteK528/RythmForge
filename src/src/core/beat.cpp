#include "../../include/core/beat.hpp"
#include <iostream>
#include "../../include/core/core.hpp"
//std::vector<double> beat(const rythm_forge::PCMData& pcm){
//    int startBPM = 0;
//    int tightness = 400;
//    double temposd = 1.0;
//    int alpha = 0;
//    std::vector<double> onsetEnv{};
//
//    std::vector<double> beats{};
//    int tempoMean = 240;
//
//
//
//}
namespace rythm_forge::beat {
// Convert frequency to Mel scale
    double hz_to_mel(double freq) {
        return 2595.0 * std::log10(1.0 + freq / 700.0);
    }

// Convert Mel scale to frequency
    double mel_to_hz(double mel) {
        return 700.0 * (std::pow(10.0, mel / 2595.0) - 1.0);
    }


    std::unique_ptr<d2array> create_mel_filter_bank(uint32_t sampleRate, uint32_t nFft, uint32_t nMels) {

        std::unique_ptr<d2array> melFilterBank = std::make_unique<d2array>(boost::extents[nMels][nFft / 2 + 1]);
        double fMin = 0.0;
        double fMax = sampleRate / 2.0;

        double melMin = hz_to_mel(fMin);
        double melMax = hz_to_mel(fMax);

        std::vector<double> melPoints(nMels + 2);
        for (uint32_t i = 0; i < nMels + 2; ++i) {
            melPoints[i] = melMin + (melMax - melMin) / (nMels + 1) * i;
        }

        std::vector<double> hzPoints(nMels + 2);
        for (uint32_t i = 0; i < nMels + 2; ++i) {
            hzPoints[i] = mel_to_hz(melPoints[i]);
        }

        std::vector<int> binPoints(nMels + 2);
        for (uint32_t i = 0; i < nMels + 2; ++i) {
            binPoints[i] = static_cast<int>(std::floor((nFft + 1) * hzPoints[i] / sampleRate));
        }
        for (uint32_t i = 1; i <= nMels; ++i) {
            for (int32_t j = binPoints[i - 1]; j < binPoints[i]; ++j) {
                (*melFilterBank)[i - 1][j] =
                        (j - binPoints[i - 1]) / static_cast<double>(binPoints[i] - binPoints[i - 1]);
            }
            for (int32_t j = binPoints[i]; j < binPoints[i + 1]; ++j) {
                (*melFilterBank)[i - 1][j] =
                        (binPoints[i + 1] - j) / static_cast<double>(binPoints[i + 1] - binPoints[i]);
            }
        }
        return melFilterBank;
    }
}