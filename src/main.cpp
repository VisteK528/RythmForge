#include <iostream>
#include "include/load/load_wave.hpp"
#include "include/load/pcm.hpp"
#include "include/core/fft.hpp"

int main() {
    std::ifstream file;

    file.open("data/arcade.wav", std::ios::binary);

    rythm_forge::audio::load::WaveLoader loader;
    auto optional_data = loader.loadPCMData(file);
    if(!optional_data.has_value()){
        throw std::runtime_error("An error has occured!");
    }

    rythm_forge::PCMData data = optional_data.value();
    file.close();

    std::unique_ptr<rythm_forge::c3array> stft_matrix = rythm_forge::fft::stft(data.getSamples(), 2048,
                                                                               512, 2048,
                                                                               false);

    std::unique_ptr<rythm_forge::d2array> y_hat = rythm_forge::fft::istft(stft_matrix, 2048,
                                                                               512, 2048,
                                                                               false);

    return 0;
}
