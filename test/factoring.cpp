#include "math/factor.hpp"
#include <catch.hpp>
#include <stdio.h>

TEST_CASE( "Trial Division", "[math]" ) {
    math::factor::factor_list<int> factors15 = {{3, 1}, {5, 1}};
    math::factor::factor_list<int> factors16 = {{2, 4}};
    math::factor::factor_list<int> factors17 = {{17, 1}};
    math::factor::factor_list<int> factors210 = {{2, 1}, {3, 1}, {5, 1}, {7, 1}};
    math::factor::factor_list<int> factors8051 = {{83, 1}, {97, 1}};
    math::factor::factor_list<int> factors248832 = {{2, 10}, {3, 5}};
    math::factor::factor_list<int> factors1024 = {{2, 10}};
    math::factor::factor_list<int> empty_list = {};

    int n;
    // Simple factoring tests
    n = 15;
    CHECK( math::factor::trial_division( n ) == factors15 );
    CHECK( n == 1 );

    n = 16;
    CHECK( math::factor::trial_division( n ) == factors16 );
    CHECK( n == 1 );

    n = 17;
    CHECK( math::factor::trial_division( n ) == factors17 );
    CHECK( n == 1 );

    n = 210;
    CHECK( math::factor::trial_division( n ) == factors210 );
    CHECK( n == 1 );

    n = 8051;
    CHECK( math::factor::trial_division( n ) == factors8051 );
    CHECK( n == 1 );

    n = 248832;
    CHECK( math::factor::trial_division( n ) == factors248832 );
    CHECK( n == 1 );

    // Limited factoring tests

    // p_23 == p[22] == 83, so the algorithm must be able to factor 8051 completely.
    n = 8051;
    CHECK( math::factor::trial_division( n, 23 ) == factors8051 );
    CHECK( n == 1 );

    /* Altough 248832 have only 2 and 3 as prime factors
     * (so we are in a similar situation as 8051),
     * after factoring out 1024 we are left with the number 3^5 == 243,
     * which is larger than the square of 2.
     * So, we cannot prove the factorization is complete.
     */
    n = 248832;
    CHECK( math::factor::trial_division( n, 1 ) == factors1024 );
    CHECK( n == 243 );

    // However, limiting it to 2 primes must be enough.
    n = 248832;
    CHECK( math::factor::trial_division( n, 2 ) == factors248832 );
    CHECK( n == 1 );

    /* 293 is the first prime after 17*17, so the trial division must fail.
     * (17 is the 7th prime number.)
     */
    n = 17 * 293;
    CHECK( math::factor::trial_division( n, 7 ) == factors17 );
    CHECK( n == 293 );

    /* Altough the product 19*19 is a composite number,
     * testing only the first 7 primes makes the trial division unable to prove
     * the factorization is complete.
     */
    n = 17*19*19;
    CHECK( math::factor::trial_division( n, 7 ) == factors17 );
    CHECK( n == 19*19 );

    /* Even tough the algorithm tests only up to p_3 == 5,
     * it is able to prove 17 is prime.
     */
    n = 17;
    CHECK( math::factor::trial_division( n, 3 ) == factors17 );
    CHECK( n == 1 );


    // But it cannot prove 7*7 == 49 > 25 composite.
    n = 49;
    CHECK( math::factor::trial_division( n, 3 ) == empty_list );
    CHECK( n == 49 );
}

TEST_CASE( "Utilities" ) {
    SECTION( "add_factor" ) {
        using math::factor::factor_list;
        factor_list<int> list;
        factor_list<int> list1 = {{11, 1}};
        factor_list<int> list2 = {{11, 2}};
        factor_list<int> list3 = {{5, 1}, {11, 2}};
        factor_list<int> list4 = {{3, 1}, {5, 1}, {11, 2}};
        factor_list<int> list5 = {{3, 1}, {5, 1}, {7, 1}, {11, 2}};
        factor_list<int> list6 = {{3, 1}, {5, 1}, {7, 1}, {11, 2}, {13, 1}};
        factor_list<int> list7 = {{3, 1}, {5, 2}, {7, 1}, {11, 2}, {13, 1}};
        factor_list<int> list8 = {{3, 1}, {5, 3}, {7, 1}, {11, 2}, {13, 1}};

        math::factor::add_factor( list, 11 );
        CHECK( list == list1 );

        math::factor::add_factor( list, 11 );
        CHECK( list == list2 );

        math::factor::add_factor( list, 5 );
        CHECK( list == list3 );

        math::factor::add_factor( list, 3 );
        CHECK( list == list4 );

        math::factor::add_factor( list, 7 );
        CHECK( list == list5 );

        math::factor::add_factor( list, 13 );
        CHECK( list == list6 );

        math::factor::add_factor( list, 5 );
        CHECK( list == list7 );

        math::factor::add_factor( list, 5 );
        CHECK( list == list8 );
    }
}
