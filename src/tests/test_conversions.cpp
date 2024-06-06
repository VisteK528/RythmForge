#include "catch.hpp"
#include "include/core/feature.hpp"
using namespace rythm_forge::feature;
double epsilon = 0.0001;
TEST_CASE("Test hzToMel conversion") {
    SECTION("Test with regular values") {
            REQUIRE(std::abs(hzToMel(10))-15.98600108 < epsilon);
            REQUIRE(std::abs(hzToMel(100))-150.4891024<  epsilon);
            REQUIRE(std::abs(hzToMel(1000))-999.9855371 < epsilon);
            REQUIRE(std::abs(hzToMel(10000))-3073.221489 < epsilon);
            REQUIRE(std::abs(hzToMel(44100))-4687.037032488187 < epsilon);
            REQUIRE(std::abs(hzToMel(22500))-3945.41190 < epsilon);

    }

    SECTION("Test edge cases") {
        REQUIRE(std::abs(melToHz(0))-0.0< epsilon);
        REQUIRE(std::abs(melToHz(0.0))-0.0< epsilon);
        REQUIRE(std::abs(melToHz(1))-1.6088427864 < epsilon);
        REQUIRE(std::abs(melToHz(1.0))-1.6088427864 < epsilon);
        REQUIRE(std::abs(melToHz(0.1))-1.6088427864< epsilon);
        REQUIRE(std::abs(melToHz(-1))-1.6111427769 < epsilon);
        REQUIRE(std::abs(melToHz(-10))-16.216023373 < epsilon);
    }


}

TEST_CASE("Test melToHz conversion") {
    SECTION("Test with known values") {
        REQUIRE(std::abs(melToHz(0) )- 0.0<epsilon);
        REQUIRE(std::abs(melToHz(10))- 6.238850423239239<epsilon);
        REQUIRE(std::abs(melToHz(5))-3.1125054329979696 <epsilon);
        REQUIRE(std::abs(melToHz(20))-12.533305495912206 <epsilon);
        REQUIRE(std::abs(melToHz(-1))-0.6208457357697261 <epsilon);
        REQUIRE(std::abs(melToHz(-10))-6.183736980272747 <epsilon);
        REQUIRE(std::abs(melToHz(0.5))-0.3106295113860291 <epsilon);
        REQUIRE(std::abs(melToHz(22/5))- 14.115665470102146<epsilon);
        REQUIRE(std::abs(melToHz(7.8))- 4.861549658089737<epsilon);


    }


}
