#include "catch.hpp"
#include "include/load/pcm.hpp"

TEST_CASE("Test duration estimation"){
    std::unique_ptr<rythm_forge::d2array> samples = std::make_unique<rythm_forge::d2array>(boost::extents[10][2]);
    for(uint8_t i = 0; i < 10; ++i){
        (*samples)[i][0] = 5. + 10. * i;
        (*samples)[i][0] = 100. - 10. * i;
    }
    REQUIRE(samples->size() == 10);

    unsigned int sampleRate = 44100;
    rythm_forge::PCMData pcmData(std::move(samples), sampleRate);

    REQUIRE(pcmData.getSampleRate() == 44100);

    double expected_duration = 0.000226757;
    REQUIRE_THAT(pcmData.getDurationTime(), Catch::Matchers::WithinRel(expected_duration, 0.001)
                                            || Catch::Matchers::WithinAbs(0, 0.000001));
}

TEST_CASE("Test sample averaging across channels"){
    std::unique_ptr<rythm_forge::d2array> samples = std::make_unique<rythm_forge::d2array>(boost::extents[5][2]);
    double data[] = {
            10, 20, 15, 25, 20, 20, 10, 10, 2, 8
    };
    samples->assign(data, data+10);
    REQUIRE(samples->size() == 5);

    unsigned int sampleRate = 44100;
    rythm_forge::PCMData pcmData(std::move(samples), sampleRate);
    pcmData.toMono();

    // Check sample array dimensions after averaging
    REQUIRE(pcmData.getSamples()->shape()[1] == 1);

    // Check new samples values
    REQUIRE((*pcmData.getSamples())[0][0] == 15);
    REQUIRE((*pcmData.getSamples())[1][0] == 20);
    REQUIRE((*pcmData.getSamples())[2][0] == 20);
    REQUIRE((*pcmData.getSamples())[3][0] == 10);
    REQUIRE((*pcmData.getSamples())[4][0] == 5);
}

