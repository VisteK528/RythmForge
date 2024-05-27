#include "../../include/core/fft.hpp"

std::vector<cx> fft(std::vector<cx> a){
    if(a.size() == 1){
        return a;
    }
    else{
        long m = round(a.size() / 2);

        std::vector<cx> even;
        std::vector<cx> odd;
        even.reserve(m);
        odd.reserve(m);

        // Copy even
        long index = -1;
        std::copy_if(a.begin(), a.end(), std::back_inserter(even), [&index](cx){++index; return index % 2 == 0;});

        // Copy odd
        index = -1;
        std::copy_if(a.begin(), a.end(), std::back_inserter(odd), [&index](cx){++index; return index % 2 == 1;});

        // Recursive call
        std::vector<cx> even_result;
        std::vector<cx> odd_result;
        even_result.reserve(m);
        odd_result.reserve(m);

        even_result = fft(even);
        odd_result = fft(odd);

        cx w = std::exp(cx(0, -2*M_PI/static_cast<double>(a.size())));

        std::vector<cx> result;
        result.resize(2*m);
        std::fill(result.begin(), result.end(), 0);
        cx h = 1;

        for(long i = 0; i < m; ++i){
            result[i] = even_result[i] + h * odd_result[i];
            result[i+m] = even_result[i] - h * odd_result[i];
            h *= w;
        }

        return result;
    }
}

std::vector<cx> ifft(std::vector<cx> a){
    std::vector<cx> swiped;
    swiped.reserve(a.size());

    for(const auto& sample: a){
        swiped.push_back(cx(sample.imag(), sample.real()));
    }

    std::vector<cx> fft_result;
    fft_result.reserve(a.size());
    fft_result = fft(swiped);


    std::vector<cx> swiped_again;
    swiped_again.reserve(a.size());
    for(const auto& sample: fft_result){
        swiped_again.push_back(cx(sample.imag(), sample.real()));
    }
    return swiped_again;
}
