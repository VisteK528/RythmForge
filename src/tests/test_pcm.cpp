#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/core/pcm.hpp"

TEST_CASE("Test duration estimation"){
    std::unique_ptr<std::vector<std::vector<double>>> samples = std::make_unique<std::vector<std::vector<double>>>();
    for(uint8_t i = 0; i < 10; ++i){
        samples->push_back({static_cast<double>(5+10*i), static_cast<double>(100-i*10)});
    }
    REQUIRE(samples->size() == 10);

    unsigned int sampleRate = 44100;
    PCMData pcmData(std::move(samples), sampleRate);

    REQUIRE(pcmData.getSampleRate() == 44100);

    double expected_duration = 0.000226757;
    REQUIRE_THAT(pcmData.getDurationTime(), Catch::Matchers::WithinRel(expected_duration, 0.001)
                                            || Catch::Matchers::WithinAbs(0, 0.000001));
}

