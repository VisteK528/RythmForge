#include <iostream>
#include "include/load/load_wave.hpp"
#include "include/load/pcm.hpp"
#include "include/core/fft.hpp"
#include "include/core/beat.hpp"

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
    rythm_forge::beat::tempo(data);


    return 0;
}
