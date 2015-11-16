#include "pinch/shares.hpp"
#include <catch.hpp>

#include <sstream>

TEST_CASE( "pinch::share input and output", "[pinch]" ) {
    std::stringstream stream;
    pinch::share<int> share;

    stream.str("1 6");
    CHECK( stream >> share );
    CHECK( share.id == 1 );
    CHECK( share.share == 6 );

    share.id = 2;
    share.share = 8;
    stream.str("");
    stream.clear();
    stream << share;
    CHECK( stream.str() == "2 8" );
}
