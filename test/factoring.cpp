#include "math/factor.hpp"
#include <catch.hpp>
#include <stdio.h>

TEST_CASE( "Trial Division", "[math]" ) {
    std::vector< std::pair<int, int> > factors15 = {{3, 1}, {5, 1}};
    std::vector< std::pair<int, int> > factors16 = {{2, 4}};
    std::vector< std::pair<int, int> > factors17 = {{17, 1}};
    std::vector< std::pair<int, int> > factors210 = {{2, 1}, {3, 1}, {5, 1}, {7, 1}};
    std::vector< std::pair<int, int> > factors8051 = {{83, 1}, {97, 1}};
    std::vector< std::pair<int, int> > factors248832 = {{2, 10}, {3, 5}};

    CHECK( math::factor::trial_division( 15 ) == factors15 );
    CHECK( math::factor::trial_division( 16 ) == factors16 );
    CHECK( math::factor::trial_division( 17 ) == factors17 );
    CHECK( math::factor::trial_division( 210 ) == factors210 );
    CHECK( math::factor::trial_division( 8051 ) == factors8051 );
    CHECK( math::factor::trial_division( 248832 ) == factors248832 );
}
