#ifndef RYTHMFORGE_CORE_HPP
#define RYTHMFORGE_CORE_HPP

#include <boost/multi_array.hpp>

namespace rythm_forge{
    using byte = unsigned char;
    using c3array = boost::multi_array<double, 3>;
    using d2array = boost::multi_array<double, 2>;
    using dcomplex = std::complex<double>;

    namespace fft{}
    namespace audio{
        namespace load{}
        namespace convert{}
    }
}

#endif //RYTHMFORGE_CORE_HPP
