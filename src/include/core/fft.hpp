#ifndef RYTHMFORGE_FFT_HPP
#define RYTHMFORGE_FFT_HPP

#include <complex>
#include <vector>

using cx = std::complex<double>;

std::vector<cx> fft(std::vector<cx> a);
std::vector<cx> ifft(std::vector<cx> a);

#endif //RYTHMFORGE_FFT_HPP
