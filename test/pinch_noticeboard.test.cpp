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

TEST_CASE( "group_data failed read", "[pinch]" ) {
    pinch::group_data<int> data;
    std::stringstream stream;

    /* All these calls should not throw an exception due to
     * calling vector.resize with memory garbage. */
    CHECK_FALSE( stream >> data );
    CHECK_FALSE( stream >> data );
    CHECK_FALSE( stream >> data );
    CHECK_FALSE( stream >> data );
}

TEST_CASE( "noticeboard input and output", "[pinch]" ) {
    pinch::noticeboard<int> board;
    std::stringstream stream;

    stream.str( "2 19\n25 36 2 1 2\n78 89 2 1 3\n" );
    CHECK( stream >> board );
    CHECK( board.generator == 2 );
    CHECK( board.prime_modulo == 19 );
    REQUIRE( board.groups.size() == 2 );
    CHECK( board.groups[0].group_generator == 25 );
    CHECK( board.groups[0].group_value == 36 );
    REQUIRE( board.groups[0].group.size() == 2 );
    CHECK( board.groups[0].group[0] == 1 );
    CHECK( board.groups[0].group[1] == 2 );
    CHECK( board.groups[1].group_generator == 78 );
    CHECK( board.groups[1].group_value == 89 );
    REQUIRE( board.groups[1].group.size() == 2 );
    CHECK( board.groups[1].group[0] == 1 );
    CHECK( board.groups[1].group[1] == 3 );

    stream.str("");
    stream.clear();
    stream << board;
    CHECK( stream.str() == "2 19\n25 36 2 1 2\n78 89 2 1 3\n" );
}

TEST_CASE( "noticeboard retrieval", "[pinch]" ) {
    pinch::noticeboard<int> board;
    std::stringstream stream;

    stream.str( "2 19\n25 36 2 1 2\n78 89 2 1 3\n" );
    stream >> board;

    pinch::group_data<int> data;
    REQUIRE_NOTHROW( data = board.retrieve_data( {1, 2} ) );
    CHECK( data.group_generator == 25 );

    REQUIRE_NOTHROW( data = board.retrieve_data( {1, 3} ) );
    CHECK( data.group_generator == 78 );

    REQUIRE_THROWS_AS( data = board.retrieve_data( {2, 3} ), std::out_of_range );
}
