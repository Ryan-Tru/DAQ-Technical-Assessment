#include <catch2/catch_test_macros.hpp>
#include </app/solution/canParser.h>

TEST_CASE("Passing tests for convert to hex", "[convertToHex]") {
    REQUIRE(convertToHex('0') == 0);
    REQUIRE(convertToHex('9') == 9);
    REQUIRE(convertToHex('A') == 10);
    REQUIRE(convertToHex('F') == 15);
    REQUIRE(convertToHex('a') == 10);
    REQUIRE(convertToHex('f') == 15); 
}

TEST_CASE("Failure tests for convert to hex", "[convertToHex]") {
    REQUIRE_THROWS(convertToHex('G'));
    REQUIRE_THROWS(convertToHex('%'));
    REQUIRE_THROWS(convertToHex('@'));
    REQUIRE_THROWS(convertToHex('z'));
}

TEST_CASE("Empty DBC file", "[loadCanFrames]") {
    REQUIRE_THROWS(loadCanFrames("nonExistent.dbc"));

    std::ofstream empty("empty.dbc");
    empty.close();
    REQUIRE_THROWS(loadCanFrames("empty.dbc"));
    std::remove("empty.dbc");
}
