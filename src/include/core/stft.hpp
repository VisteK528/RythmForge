#ifndef RYTHMFORGE_STFT_HPP
#define RYTHMFORGE_STFT_HPP

#include <boost/multi_array.hpp>
#include <memory>
#include "core.hpp"

namespace rythm_forge::fft{
    std::unique_ptr<c3array> stft();
}


#endif //RYTHMFORGE_STFT_HPP
