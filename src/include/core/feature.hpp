#ifndef RYTHMFORGE_FEATURE_H
#define RYTHMFORGE_FEATURE_H
#include "core.hpp"
namespace rythm_forge::feature {
    template<typename T>
        requires std::convertible_to<T, double>
    double hzToMel(T frequency) {
        return 2595 * std::log10(1.0 + (double) frequency / 700);
    }
    template<typename T>
        requires std::convertible_to<T, double>
    double melToHz(T mel) {
        return 700 * (std::pow(10, mel / 2595) - 1);
    }

    std::unique_ptr<d2array> create_mel_filter_bank(uint32_t sampleRate, uint32_t nFft, uint32_t nMels);
    std::vector<int> find_pick(std::vector<double> onset_envelope);
class EmptyVector:public std::exception{
private:
    std::string msg;
public:
    explicit EmptyVector(const std::string& message):msg(message){}
    [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }
};
}// namespace rythm_forge::feature
#endif//RYTHMFORGE_FEATURE_H
