#include "math/algo.hpp"
#include <catch.hpp>

TEST_CASE( "Greatest Common Divisor", "[math]" ) {
    CHECK( math::gcd( 2, 3 ) == 1 );
    CHECK( math::gcd( 3, 3 ) == 3 );
    CHECK( math::gcd( 2, 0 ) == 2 );
    CHECK( math::gcd( 4, 6 ) == 2 );
    CHECK( math::gcd( 14, 49 ) == 7 );
    CHECK( math::gcd( 14, 49 ) == 7 );
    CHECK( math::gcd( 144, 89 ) == 1 );
}
