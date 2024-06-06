#include "catch.hpp"
#include <vector>
#include <iostream>
#include "include/core/feature.hpp"
using namespace rythm_forge::feature;
TEST_CASE("Test find_pick function") {
    SECTION("Test with typical input") {
        std::vector<double> onset_envelope = {1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0};
        std::vector<int> expected_peaks = {1,4,7};
        REQUIRE(find_pick(onset_envelope) == expected_peaks);


        onset_envelope = {1.0, 2.0, 3.0, 3.0, 2.0, 1.0};
        expected_peaks = {1,4};
        REQUIRE(find_pick(onset_envelope) == expected_peaks);
    }

    SECTION("Test with multiple peaks") {
        std::vector<double> onset_envelope = {1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 5.0, 6.0, 7.0};
        std::vector<int> expected_peaks = {1,4};
        REQUIRE(find_pick(onset_envelope) == expected_peaks);
    }


    SECTION("Test with empty input") {
        std::vector<double> onset_envelope;
        std::vector<int> expected_peaks = {};
        REQUIRE_THROWS(find_pick(onset_envelope));
    }

}