#ifndef RYTHMFORGE_FFT_HPP
#define RYTHMFORGE_FFT_HPP

#include <complex>
#include <vector>
#include "core.hpp"

namespace rythm_forge::fft{
    std::vector<dcomplex> fft(std::vector<dcomplex> a);
    std::vector<dcomplex> ifft(std::vector<dcomplex> a);
}


#endif //RYTHMFORGE_FFT_HPP
