#include "pinch/user_data.hpp"
#include <catch.hpp>
#include <sstream>

TEST_CASE( "pinch::message input and output", "[pinch]" ) {
    pinch::message<int> message;
    std::stringstream stream;

    stream.str("1 2 3 4\n2 3\n");
    CHECK( stream >> message );
    CHECK( message.group_data == 1 );
    CHECK( message.partial_message == 2 );
    CHECK( message.prime_modulo == 3 );
    CHECK( message.generator == 4 );
    REQUIRE( message.remaining_ids.size() == 2 );
    CHECK( message.remaining_ids[0] == 2 );
    CHECK( message.remaining_ids[1] == 3 );

    stream.str();
    stream.clear();
    stream << message;
    CHECK( stream.str() == "1 2 3 4\n2 3\n" );
}
