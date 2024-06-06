#include "catch.hpp"
#include "include/core/fft.hpp"
#include <boost/multi_array.hpp>
#include <complex>
#include <memory>
#include <vector>

using namespace rythm_forge::fft;
TEST_CASE("Test calculateMagnitude function") {
    SECTION("Test with typical input") {
        using ComplexArray = boost::multi_array<rythm_forge::dcomplex, 2>;
        auto complexMatrix = std::make_unique<ComplexArray>(boost::extents[2][2]);
        (*complexMatrix)[0][0] = std::complex<double>(1.0, 2.0);
        (*complexMatrix)[0][1] = std::complex<double>(-3.0, 4.0);
        (*complexMatrix)[1][0] = std::complex<double>(5.0, -6.0);
        (*complexMatrix)[1][1] = std::complex<double>(-7.0, -8.0);

        auto magnitudeMatrix = calculateMagnitude(complexMatrix);

        REQUIRE(magnitudeMatrix->shape()[0] == 2);
        REQUIRE(magnitudeMatrix->shape()[1] == 2);
        REQUIRE((*magnitudeMatrix)[0][0] == Approx(std::sqrt(5.0)));
        REQUIRE((*magnitudeMatrix)[0][1] == Approx(std::sqrt(25.0)));
        REQUIRE((*magnitudeMatrix)[1][0] == Approx(std::sqrt(61.0)));
        REQUIRE((*magnitudeMatrix)[1][1] == Approx(std::sqrt(113.0)));
    }

    SECTION("Test with empty input") {
        using ComplexArray = boost::multi_array<std::complex<double>, 2>;
        auto complexMatrix = std::make_unique<ComplexArray>(boost::extents[0][0]);

        auto magnitudeMatrix = calculateMagnitude(complexMatrix);

        // Verify that the magnitude matrix is also empty
        REQUIRE(magnitudeMatrix->shape()[0] == 0);
        REQUIRE(magnitudeMatrix->shape()[1] == 0);
    }

    SECTION("Test with typical input") {
        // Create a sample 1D complex array
        using ComplexArray = boost::multi_array<std::complex<double>, 1>;
        auto complexMatrix = std::make_unique<ComplexArray>(boost::extents[2]);
        (*complexMatrix)[0] = std::complex<double>(1.0, 2.0);
        (*complexMatrix)[1] = std::complex<double>(-3.0, 4.0);

        // Call the function and check for exception
        REQUIRE_NOTHROW(calculateMagnitude(complexMatrix));
    }



    SECTION("Test with typical input") {
        using ComplexArray = boost::multi_array<std::complex<double>, 3>;
        auto complexMatrix = std::make_unique<ComplexArray>(boost::extents[2][2][2]);
        (*complexMatrix)[0][0][0] = std::complex<double>(1.0, 2.0);
        (*complexMatrix)[0][0][1] = std::complex<double>(-3.0, 4.0);
        (*complexMatrix)[0][1][0] = std::complex<double>(5.0, -6.0);
        (*complexMatrix)[0][1][1] = std::complex<double>(-7.0, -8.0);
        (*complexMatrix)[1][0][0] = std::complex<double>(9.0, 10.0);
        (*complexMatrix)[1][0][1] = std::complex<double>(-11.0, 12.0);
        (*complexMatrix)[1][1][0] = std::complex<double>(13.0, -14.0);
        (*complexMatrix)[1][1][1] = std::complex<double>(-15.0, -16.0);

        // Call the function and check for exception
        REQUIRE_NOTHROW(calculateMagnitude(complexMatrix));
    }

}
