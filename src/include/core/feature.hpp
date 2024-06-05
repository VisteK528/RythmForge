#ifndef RYTHMFORGE_FEATURE_H
#define RYTHMFORGE_FEATURE_H
#include "core.hpp"
namespace rythm_forge::feature {
    template<typename T>
        requires std::convertible_to<T, double>
    double hzToMel(T frequency) {
        return 2595 * std::log10(1.0 + (double )frequency / 700);
    }
    template<typename T>
        requires std::convertible_to<T, double>
    double melToHz(T mel) {
        PRINT_VALUE(700 * (std::pow(10, mel / 2595) - 1));
        return 700 * (std::pow(10, mel / 2595) - 1);
    }


    std::unique_ptr<d2array> create_mel_filter_bank(uint32_t sampleRate, uint32_t nFft, uint32_t nMels);
}// namespace rythm_forge::feature
#endif//RYTHMFORGE_FEATURE_H
