#include "pinch/noticeboard.hpp"
#include <catch.hpp>
#include <sstream>

TEST_CASE( "group_data input and output", "[pinch]" ) {
    pinch::group_data<int> data;
    std::stringstream stream;

    stream.str( "15 5 3 1 2 3" );
    CHECK( stream >> data );
    CHECK( data.group_generator == 15 );
    CHECK( data.group_value == 5 );
    REQUIRE( data.group.size() == 3 );
    CHECK( data.group[0] == 1 );
    CHECK( data.group[1] == 2 );
    CHECK( data.group[2] == 3 );

    stream.str("");
    stream.clear();
    stream << data;
    CHECK( stream.str() == "15 5 3 1 2 3" );
}
