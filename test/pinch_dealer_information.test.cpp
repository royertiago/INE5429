#include "pinch/dealer_information.hpp"
#include <catch.hpp>
#include <sstream>

TEST_CASE( "pinch::dealer_information input and output", "[pinch]" ) {
    std::stringstream stream;
    pinch::dealer_information<int> dealer_information;

    stream.str( "2 17 3\n1 8\n2 7\n" );
    CHECK( stream >> dealer_information );
    CHECK( dealer_information.last_id == 2 );
    CHECK( dealer_information.prime == 17 );
    CHECK( dealer_information.generator == 3 );
    CHECK( dealer_information.valid_shares[0].id == 1 );
    CHECK( dealer_information.valid_shares[0].share == 8 );
    CHECK( dealer_information.valid_shares[1].id == 2 );
    CHECK( dealer_information.valid_shares[1].share == 7 );

    stream.str("");
    stream.clear();
    stream << dealer_information;
    CHECK( stream.str() == "2 17 3\n1 8\n2 7\n" );
}
