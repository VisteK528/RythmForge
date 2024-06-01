#ifndef RYTHMFORGE_FFT_HPP
#define RYTHMFORGE_FFT_HPP

#include <complex>
#include <vector>
#include <exception>
#include "core.hpp"

namespace rythm_forge::fft{
    std::vector<dcomplex> fft(std::vector<dcomplex> a);
    std::vector<dcomplex> ifft(std::vector<dcomplex> a);

    std::unique_ptr<c3array> stft(const std::unique_ptr<d2array>& samples, unsigned int nFft, unsigned int hopSize, unsigned int windowLength, bool center);
    std::unique_ptr<d2array> istft(const std::unique_ptr<c3array>& stft_matrix, unsigned int nFtt, unsigned int hopSize, unsigned int window_length, bool center);

    class InvalidNFFTSize;
}


class rythm_forge::fft::InvalidNFFTSize: public std::exception
{
private:
    std::string msg;
public:
    explicit InvalidNFFTSize(const std::string& message): msg(message) {}
    const char* what() const noexcept override { return msg.c_str(); }
};


#endif //RYTHMFORGE_FFT_HPP