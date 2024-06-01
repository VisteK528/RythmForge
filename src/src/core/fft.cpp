#include "../../include/core/fft.hpp"

namespace rythm_forge::fft{
    std::vector<dcomplex> fft(std::vector<dcomplex> a){
        if(a.size() == 1){
            return a;
        }
        else{
            long m = round(a.size() / 2);

            std::vector<dcomplex> even;
            std::vector<dcomplex> odd;
            even.reserve(m);
            odd.reserve(m);

            // Copy even
            long index = -1;
            std::copy_if(a.begin(), a.end(), std::back_inserter(even), [&index](dcomplex){++index; return index % 2 == 0;});

            // Copy odd
            index = -1;
            std::copy_if(a.begin(), a.end(), std::back_inserter(odd), [&index](dcomplex){++index; return index % 2 == 1;});

            // Recursive call
            std::vector<dcomplex> even_result;
            std::vector<dcomplex> odd_result;
            even_result.reserve(m);
            odd_result.reserve(m);

            even_result = fft(even);
            odd_result = fft(odd);

            dcomplex w = std::exp(dcomplex(0, -2*M_PI/static_cast<double>(a.size())));

            std::vector<dcomplex> result;
            result.resize(2*m);
            std::fill(result.begin(), result.end(), 0);
            dcomplex h = 1;

            for(long i = 0; i < m; ++i){
                result[i] = even_result[i] + h * odd_result[i];
                result[i+m] = even_result[i] - h * odd_result[i];
                h *= w;
            }

            return result;
        }
    }

    std::vector<dcomplex> ifft(std::vector<dcomplex> a){
        std::vector<dcomplex> swiped;
        swiped.reserve(a.size());

        for(const auto& sample: a){
            swiped.push_back(dcomplex(sample.imag(), sample.real()));
        }

        std::vector<dcomplex> fft_result;
        fft_result.reserve(a.size());
        fft_result = fft(swiped);


        std::vector<dcomplex> swiped_again;
        swiped_again.reserve(a.size());
        for(const auto& sample: fft_result){
            swiped_again.push_back(dcomplex(sample.imag(), sample.real()));
        }
        return swiped_again;
    }

}

