#include "include/core/feature.hpp"
#include "catch.hpp"
#include <vector>
#include <memory>
#include <cstdint>

using namespace rythm_forge::feature;
TEST_CASE("Test create_mel_filter_bank function") {
    SECTION("Test with typical input") {

        uint32_t sampleRate = 44100;
        uint32_t nFft = 1024;
        uint32_t nMels = 40;
        auto melFilterBank = create_mel_filter_bank(sampleRate, nFft, nMels);

        REQUIRE(melFilterBank->shape()[0] == nMels);
        REQUIRE(melFilterBank->shape()[1] == (nFft / 2 + 1));

    }


}
