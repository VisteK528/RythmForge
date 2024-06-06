#include "catch.hpp"
#include "include/load/pcm.hpp"
using namespace rythm_forge;
TEST_CASE("Test PCMData::resample function") {
    SECTION("Test with typical input") {
        unsigned int sampleRate = 100;
        std::unique_ptr<d2array> samples = std::make_unique<d2array>(boost::extents[sampleRate][1]);

        for (unsigned int i = 0; i < sampleRate; ++i) {
            (*samples)[i][0] = i;
        }

        PCMData pcmData(std::move(samples), sampleRate);

        unsigned int target_sr = 10;

        auto resampledData = PCMData::resample(pcmData, target_sr);

        REQUIRE(resampledData.getSampleRate() == target_sr);


    }
    SECTION("Test with typical input 2") {
        unsigned int sampleRate = 100;
        std::unique_ptr<d2array> samples = std::make_unique<d2array>(boost::extents[sampleRate][2]);
        for (unsigned int i = 0; i < sampleRate; ++i) {
            (*samples)[i][0] = i;
        }
        for (unsigned int i = 0; i < sampleRate; ++i) {
            (*samples)[i][1] = 10+2*i;
        }

        PCMData pcmData(std::move(samples), sampleRate);

        unsigned int target_sr = 10;

        auto resampledData = PCMData::resample(pcmData, target_sr);

        REQUIRE(resampledData.getSampleRate() == target_sr);


    }

}