#include "../../include/core/fft.hpp"
#include <iostream>

namespace rythm_forge::fft{

    static d2array obtainFrame(const std::unique_ptr<d2array>& samples, const  size_t windowLength, const size_t hopSize,
                               const size_t frameNumber, const size_t channel){
        size_t start = frameNumber*hopSize;
        size_t end = start + windowLength;

        if(end> samples->shape()[0]){
            end = samples->shape()[0];
        }

        d2array frame(boost::extents[end-start][1]);
        for(; start < end; ++start){
            frame[start-hopSize*frameNumber][0] = (*samples)[start][channel];
        }
        return frame;
    }

    static c2array obtainStftSubmatrix(const std::unique_ptr<c3array>& stftMatrix, const size_t numFrame,
                                       const size_t channel){
        const size_t frequencyBins = stftMatrix->shape()[1];

        c2array frame(boost::extents[2*frequencyBins][1]);

        for(size_t i = 0; i < frequencyBins; ++i){
            frame[i][0] = (*stftMatrix)[channel][i][numFrame];
            frame[frequencyBins+i][0] = conj((*stftMatrix)[channel][frequencyBins-1][numFrame]);
        }
        return frame;
    }

    static double hann_window(int n, int N) {
        return 0.5 * (1 - std::cos(2 * PI * n / (N - 1)));
    }

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

            dcomplex w = std::exp(dcomplex(0, -2*PI/static_cast<double>(a.size())));

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


    std::unique_ptr<c3array> stft(const std::unique_ptr<d2array>& samples, unsigned int nFft, unsigned int hopSize,
                                  unsigned int windowLength, bool center){

        if(std::floor(log2(nFft)) != log2(nFft)){
            throw InvalidNFFTSize("In order to use FFT algorithm the nFft has to be a power of 2!");
        }

        unsigned long samplesLength = samples->shape()[0];
        if(center){
            samplesLength += windowLength / 2;
        }

        const size_t numChannels = samples->shape()[1];
        const size_t frequencyBins = nFft / 2 + 1;
        const size_t frames = (samplesLength - windowLength) / hopSize + 1;

        std::unique_ptr<c3array> stft_matrix = std::make_unique<c3array>(
                (boost::extents[numChannels][frequencyBins][frames]));

        for(size_t i = 0; i < numChannels; ++i){
            for(size_t j = 0; j < frames; ++j){
                std::cout<<j<<"/"<<frames<<std::endl;
                d2array frame = obtainFrame(samples, windowLength, hopSize, j, i);
                const size_t frameLength = frame.shape()[0];

                // Hann window
                d2array frameHann(boost::extents[frameLength][1]);
                for(size_t k = 0; k < frameLength; ++k){
                    frameHann[k][0] = frame[k][0] * hann_window(k, frameLength);
                }

                // fft
                std::vector<dcomplex> frameVector;
                frameVector.reserve(frameLength);
                frameVector.resize(frameLength);
                std::fill(frameVector.begin(), frameVector.end(), 0.0);
                std::transform(frameHann.begin(), frameHann.end(), frameVector.begin(),
                               [](auto sample){return sample[0];});
                std::vector<dcomplex> fftResult = fft(frameVector);

                // assign
                for(size_t k = 0; k < frequencyBins; ++k){
                    (*stft_matrix)[i][k][j] = fftResult.at(k);
                }
            }
        }
        return stft_matrix;
    }


    std::unique_ptr<d2array> istft(const std::unique_ptr<c3array>& stft_matrix, unsigned int nFtt, unsigned int hopSize,
                                   [[maybe_unused]]unsigned int window_length, [[maybe_unused]]bool center){
        const size_t numChannels = stft_matrix->shape()[0];
        const size_t frequencyBins = stft_matrix->shape()[1];
        const size_t frames = stft_matrix->shape()[2];


        size_t outputLength = (frames -1) * hopSize + nFtt;

        std::unique_ptr<d2array> y = std::make_unique<d2array>(
                (boost::extents[outputLength][numChannels]));


        std::vector<double> hannWindow;
        hannWindow.resize(nFtt);
        std::generate(hannWindow.begin(), hannWindow.end(), [N=hannWindow.size(), n=0]()mutable {return 1*hann_window(++n, N);});


        for(size_t i = 0; i < numChannels; ++i){
            std::vector<double> hannWindowCoefficients;
            hannWindowCoefficients.resize(outputLength);
            for(size_t j = 0; j < frames; ++j){
                c2array frame = obtainStftSubmatrix(stft_matrix, j, i);

                // istft
                std::vector<dcomplex> frameVector;
                frameVector.reserve(2*frequencyBins);
                frameVector.resize(2*frequencyBins);
                std::fill(frameVector.begin(), frameVector.end(), 0.0);
                std::transform(frame.begin(), frame.end(), frameVector.begin(),
                               [](auto sample){return sample[0];});

                std::vector<dcomplex> istftResult = ifft(frameVector);

                // Hann window

                std::vector<double> istftResultWindow;
                istftResultWindow.reserve(istftResult.size());
                istftResultWindow.resize(istftResult.size());
                std::transform(istftResult.cbegin(), istftResult.cend(), istftResultWindow.begin(),
                               [N=istftResult.size(), n=0](auto& sample)mutable {return sample.real() * hann_window(++n, N);});

                // assign
                const size_t start = j*hopSize;
                for(size_t k = 0; k < nFtt; ++k){
                    (*y)[start+k][i] = (*y)[start+k][i] + istftResultWindow.at(k);
                    hannWindowCoefficients[start+k] = hannWindowCoefficients[start+k] + hannWindow[k];
                }
            }

            // Normalize
            for(size_t j = 0; j < outputLength; ++j){
                if(hannWindowCoefficients.at(j) != 0){
                    (*y)[j][i] = (*y)[j][i] / hannWindowCoefficients.at(j);
                }
            }
        }

        return y;
    }
}

