#include "catch.hpp"
#include "include/core/core.hpp"
#include "include/core/fft.hpp"
#include <cmath>
#include <complex>
#include <vector>

using namespace rythm_forge::fft;
using dcomplex = rythm_forge::dcomplex;
using d2array = rythm_forge::d2array;

TEST_CASE("FFT of a single-element vector", "[fft]") {
    std::vector<dcomplex> input = {{1.0, 0.0}};
    std::vector<dcomplex> output = fft(input);
    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == input[0]);
}

TEST_CASE("IFFT of a single-element vector", "[ifft]") {
    std::vector<dcomplex> input = {{1.0, 0.0}};
    std::vector<dcomplex> output = ifft(input);
    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == input[0]);
}


TEST_CASE("STFT with zero padding", "[stft]") {
    std::unique_ptr<d2array> samples = std::make_unique<d2array>(boost::extents[1024][1]);
    for (size_t i = 0; i < 1024; ++i) {
        (*samples)[i][0] = std::sin(2 * PI * i / 64);
    }

    auto stft_matrix = stft(samples, 256, 128, 256, true);
    REQUIRE(stft_matrix->shape()[2] == 8);
}

TEST_CASE("Invalid NFFT size for STFT", "[stft]") {
    std::unique_ptr<d2array> samples = std::make_unique<d2array>(boost::extents[1024][1]);
    REQUIRE_THROWS_AS(stft(samples, 250, 128, 256, false), InvalidNFFTSize);
}
