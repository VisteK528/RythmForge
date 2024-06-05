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
                return (i - 1) * ((double )sampleRate / nFft);
            };
            std::generate(fftFrequencies.begin(), fftFrequencies.end(), generator);
        }
        std::vector<double> melFrequencies(nMels+2);
        {
            uint16_t i = 0;
            auto generator = [&]() {
                ++i;
                return melToHz(((melMin + (melMax - melMin)) / (nMels + 1.0)) * (i-1.0));
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
                return 2.0 / (melFrequencies[3+i] - melFrequencies[i - 1]);
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

//
//std::unique_ptr<d2array> create_mel_filter_bank(int sampleRate, int nFft, int nMels) {
//    std::unique_ptr<d2array> melFilterBank = std::make_unique<d2array>(boost::extents[nMels][nFft / 2 + 1]);
//    // Compute the Mel points
//    double lowFreqMel = hzToMel(0);
//    double highFreqMel = hzToMel(sampleRate / 2);
//    std::vector<double> melPoints(nMels + 2);
//    for (int i = 0; i < nMels + 2; ++i) {
//        melPoints[i] = lowFreqMel + (highFreqMel - lowFreqMel) * i / (nMels + 1);
//    }
//
//    // Convert Mel points to frequency points
//    std::vector<double> binPoints(nMels + 2);
//    for (int i = 0; i < nMels + 2; ++i) {
//        binPoints[i] = std::floor((nFft + 1) * melToHz(melPoints[i]) / sampleRate);
//    }
//
//    // Initialize the filter bank
////    std::vector<std::vector<double>> filterBank(nMels, std::vector<double>(nFFT / 2 + 1, 0.0));
//
//    // Populate the filter bank with triangular filters
//    for (int i = 1; i <= nMels; ++i) {
//        int left = binPoints[i - 1];
//        int center = binPoints[i];
//        int right = binPoints[i + 1];
//
//        for (int j = left; j < center; ++j) {
//            (*melFilterBank)[i - 1][j] = (j - (double )binPoints[i - 1]) / (binPoints[i] - binPoints[i - 1]);
//        }
//        for (int j = center; j < right; ++j) {
//            (*melFilterBank)[i - 1][j] = (binPoints[i + 1] - j) / (binPoints[i + 1] - binPoints[i]);
//        }
//    }
//
//    return melFilterBank;
//}
}// namespace rythm_forge::feature