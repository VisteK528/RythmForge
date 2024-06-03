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

    std::unique_ptr<d3array> apply_mel_filter_bank(std::unique_ptr<d3array> &magnitudeMatrix, std::unique_ptr<d2array> &mel_filter_bank) {
        uint64_t n_mels = mel_filter_bank->shape()[0];
        uint64_t n_frames = magnitudeMatrix->shape()[0];
        std::unique_ptr<d3array > mel_spectogram = std::make_unique<d3array>(boost::extents[magnitudeMatrix->shape()[0]][n_mels][n_frames]);
        for (uint64_t i = 0; i < magnitudeMatrix->shape()[0]; ++i) {
            for (uint64_t j = 0; j < n_mels; ++j) {
                for (uint64_t k = 0; k < n_frames; ++k) {
                    for (uint64_t l = 0; l < magnitudeMatrix->shape()[0]; ++l) {
                        mel_spectogram->operator[](i)[j][k] += mel_filter_bank->operator[](j)[l] * magnitudeMatrix->operator[](i)[l][k];
                    }
                }
            }
        }
        return mel_spectogram;
    }

    std::unique_ptr<d2array>create_mel_filter_bank(rythm_forge::PCMData &pcmData, int n_fft, int n_mels) {
        int n_freq = n_fft/2+1;
        double min_mel = hz_to_mel(0);
        double max_mel = hz_to_mel(pcmData.getSampleRate()/2);

        std::vector<double> mel_points(n_mels+2);
        std::vector<int> bin(n_mels+2);
        std::unique_ptr<d2array> mel_filter_bank = std::make_unique<d2array>(boost::extents[n_mels][n_freq]);

        for (int i = 0; i<n_mels+2;++i){
            mel_points[i] = mel_to_hz(min_mel+(max_mel-min_mel)*i/(n_mels+2));
            bin[i] = std::floor((n_fft+1)*mel_points[i]/pcmData.getSampleRate());

        }
        for (int i = 1; i < n_mels + 1; ++i) {
            for (int j = bin[i - 1]; j < bin[i]; ++j) {
            mel_filter_bank->operator[](i-1)[j]= (j - bin[i - 1]) / (double)(bin[i] - bin[i - 1]);

            }
            for (int j = bin[i]; j < bin[i + 1]; ++j) {
                mel_filter_bank->operator[](i - 1)[j] = 1.0 - ((j - bin[i]) / (double)(bin[i + 1] - bin[i]));

            }
        }
        return mel_filter_bank;
    }

    void tempo(const rythm_forge::PCMData &pcm) {
//    int tmean = 110;
//    double tsd = 0.9;
//        unsigned int sr = pcm.getSampleRate();
        std::vector<double> onsetEnv{};
        unsigned int sro = 8000;
        int swin = 1024;
        int shop = 256;
        int n_mel = 40;
//    double oesr = sro/shop;
//    double acmax = round(4*oesr);

        PCMData newPCM = rythm_forge::PCMData::resample(pcm, sro);
        std::unique_ptr<c3array> spectogram = rythm_forge::fft::stft(newPCM.getSamples(), 256, swin, shop, true);
        std::unique_ptr<d3array> magnitude = fft::calculateMagnitude(spectogram);
        std::unique_ptr<d2array> mel_filter_bank = create_mel_filter_bank(newPCM,256,n_mel);
        auto r = apply_mel_filter_bank(magnitude,mel_filter_bank);

    }
}