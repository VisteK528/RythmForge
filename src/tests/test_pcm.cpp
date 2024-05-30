#include "catch.hpp"
#include "include/load/pcm.hpp"

//TEST_CASE("Test duration estimation"){
//    std::unique_ptr<std::vector<std::vector<double>>> samples = std::make_unique<std::vector<std::vector<double>>>();
//    for(uint8_t i = 0; i < 10; ++i){
//        samples->push_back({static_cast<double>(5+10*i), static_cast<double>(100-i*10)});
//    }
//    REQUIRE(samples->size() == 10);
//
//    unsigned int sampleRate = 44100;
//    PCMData pcmData(std::move(samples), sampleRate);
//
//    REQUIRE(pcmData.getSampleRate() == 44100);
//
//    double expected_duration = 0.000226757;
//    REQUIRE_THAT(pcmData.getDurationTime(), Catch::Matchers::WithinRel(expected_duration, 0.001)
//                                            || Catch::Matchers::WithinAbs(0, 0.000001));
//}
//
//TEST_CASE("Test sample averaging across channels"){
//    std::unique_ptr<std::vector<std::vector<double>>> samples = std::make_unique<std::vector<std::vector<double>>>();
//    samples->push_back({10, 20});
//    samples->push_back({15, 25});
//    samples->push_back({20, 20});
//    samples->push_back({10, 10});
//    samples->push_back({2, 8});
//    REQUIRE(samples->size() == 5);
//
//    unsigned int sampleRate = 44100;
//    PCMData pcmData(std::move(samples), sampleRate);
//    pcmData.toMono();
//
//    // Check sample array dimensions after averaging
//    REQUIRE(pcmData.getSamples()->at(0).size() == 1);
//
//    // Check new samples values
//    REQUIRE(pcmData.getSamples()->at(0).at(0) == 15);
//    REQUIRE(pcmData.getSamples()->at(1).at(0) == 20);
//    REQUIRE(pcmData.getSamples()->at(2).at(0) == 20);
//    REQUIRE(pcmData.getSamples()->at(3).at(0) == 10);
//    REQUIRE(pcmData.getSamples()->at(4).at(0) == 5);
//}

