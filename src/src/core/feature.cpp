#include "../../include/core/feature.hpp"

namespace rythm_forge::feature {
    std::unique_ptr<rythm_forge::d2array> create_mel_filter_bank(uint32_t sampleRate, uint32_t nFft, uint32_t nMels) {

        uint32_t fMax = sampleRate / 2;
        uint32_t fMin = 0;
        double melMax = hzToMel(fMax);
        double melMin = hzToMel(fMin);
        std::unique_ptr<d2array> melFilterBank = std::make_unique<d2array>(boost::extents[nMels][nFft / 2 + 1]);

        std::vector<double> fftFrequencies(nFft / 2 + 1);
        {
            uint16_t i = 0;
            auto generator = [&]() {
                ++i;
                return (i - 1) * ((double) sampleRate / nFft);
            };
            std::generate(fftFrequencies.begin(), fftFrequencies.end(), generator);
        }
        std::vector<double> melFrequencies(nMels + 2);
        {
            uint16_t i = 0;
            auto generator = [&]() {
                ++i;
                return melToHz(((melMin + (melMax - melMin)) / (nMels + 1.0)) * (i - 1.0));
            };
            std::generate(melFrequencies.begin(), melFrequencies.end(), generator);
        }

        std::vector<double> frequencyDiff(melFrequencies.size());
        std::adjacent_difference(melFrequencies.begin(), melFrequencies.end(), frequencyDiff.begin());
        frequencyDiff.erase(frequencyDiff.begin());
        boost::multi_array<double, 2> ramps(boost::extents[nMels + 2][1 + nFft / 2]);

        for (uint32_t i = 0; i < nMels + 2; ++i) {
            for (uint32_t j = 0; j < 1 + nFft / 2; ++j) {
                ramps[i][j] = melFrequencies[i] - fftFrequencies[j];
            }
        }

        for (uint32_t i = 0; i < nMels; ++i) {
            for (uint32_t j = 0; j < 1 + nFft / 2; ++j) {
                double lower = -ramps[i][j] / frequencyDiff[i];
                double upper = ramps[i + 2][j] / frequencyDiff[i + 1];
                (*melFilterBank)[i][j] = std::max(0.0, std::min(lower, upper));
            }
        }

        std::vector<double> enorm(nMels);
        {
            uint16_t i = 0;
            auto generator = [&]() {
                ++i;
                return 2.0 / (melFrequencies[3 + i] - melFrequencies[i - 1]);
            };
            std::generate(enorm.begin(), enorm.end(), generator);
        }

        for (uint32_t i = 0; i < nMels; ++i) {
            for (uint32_t j = 0; j < 1 + nFft / 2; ++j) {
                (*melFilterBank)[i][j] *= enorm[i];
            }
        }
        return melFilterBank;
    }
}// namespace rythm_forge::feature