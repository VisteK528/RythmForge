#ifndef RYTHMFORGE_FFT_HPP
#define RYTHMFORGE_FFT_HPP

#include "core.hpp"
#include <complex>
#include <exception>
#include <vector>
#include <iterator>
#include <algorithm>

namespace rythm_forge::fft {
    const double PI = 3.14159265358979323846;
    std::vector<dcomplex> fft(std::vector<dcomplex> a);
    std::vector<dcomplex> ifft(std::vector<dcomplex> a);

    std::unique_ptr<c3array> stft(const std::unique_ptr<d2array> &samples, unsigned int nFft, unsigned int hopSize, unsigned int windowLength, bool center);
    std::unique_ptr<d2array> istft(const std::unique_ptr<c3array> &stft_matrix, unsigned int nFtt, unsigned int hopSize, unsigned int window_length, bool center);

    //    std::unique_ptr<d3array> calculateMagnitude(const std::unique_ptr<c3array> &stft_matrix);
    template<size_t N>
    std::unique_ptr<boost::multi_array<double, N>> calculateMagnitude(const std::unique_ptr<boost::multi_array<std::complex<double>, N>> &complexMatrix) {
        boost::array<typename boost::multi_array<double, N>::index, N> shape;
        std::copy(complexMatrix->shape(), complexMatrix->shape() + N, shape.begin());
        std::unique_ptr<boost::multi_array<double, N>> magnitudeMatrix = std::make_unique<boost::multi_array<double, N>>(shape);
//        std::transform(complexMatrix->begin(), complexMatrix->end(), magnitudeMatrix->begin(), [](const std::complex<double> &c)-> double {return std::abs(c);});
        std::transform(complexMatrix->data(), complexMatrix->data() + complexMatrix->num_elements(),
                       magnitudeMatrix->data(), [](const std::complex<double>& c) -> double {
                           return std::abs(c);
                       });
        return magnitudeMatrix;
    }
    class InvalidNFFTSize;
}// namespace rythm_forge::fft


class rythm_forge::fft::InvalidNFFTSize : public std::exception {
private:
    std::string msg;

public:
    explicit InvalidNFFTSize(const std::string &message) : msg(message) {}
    const char *what() const noexcept override { return msg.c_str(); }
};


#endif//RYTHMFORGE_FFT_HPP
