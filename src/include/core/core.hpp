#ifndef RYTHMFORGE_CORE_HPP
#define RYTHMFORGE_CORE_HPP

#include <boost/multi_array.hpp>
namespace rythm_forge {
    using byte = unsigned char;
    using d2array = boost::multi_array<double, 2>;
    using d3array = boost::multi_array<double, 3>;
    using dcomplex = std::complex<double>;
    using c2array = boost::multi_array<dcomplex, 2>;
    using c3array = boost::multi_array<dcomplex, 3>;

    namespace fft {}
    namespace audio {
        namespace load {}
        namespace convert {}
    }// namespace audio
}// namespace rythm_forge

#endif//RYTHMFORGE_CORE_HPP
