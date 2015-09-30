#include "math/prime_list/meta.hpp"
#include <catch.hpp>

TEST_CASE( "Metaprogrammed primality test", "[math][metaprogramming]" ) {
    using math::prime_list::is_prime;
    CHECK( is_prime<0>::value == false );
    CHECK( is_prime<1>::value == false );
    CHECK( is_prime<2>::value == true );
    CHECK( is_prime<3>::value == true );
    CHECK( is_prime<4>::value == false );
    CHECK( is_prime<5>::value == true );
    CHECK( is_prime<6>::value == false );
    CHECK( is_prime<7>::value == true );
    CHECK( is_prime<8>::value == false );
    CHECK( is_prime<9>::value == false );
    CHECK( is_prime<10>::value == false );
    CHECK( is_prime<11>::value == true );
    CHECK( is_prime<12>::value == false );
    CHECK( is_prime<13>::value == true );
    CHECK( is_prime<14>::value == false );
    CHECK( is_prime<15>::value == false );
    CHECK( is_prime<16>::value == false );
}

TEST_CASE( "Metaprogrammed next prime", "[math][metaprogramming]" ) {
    using math::prime_list::first_prime_after;
    CHECK( first_prime_after<0>::value == 2 );
    CHECK( first_prime_after<1>::value == 2 );
    CHECK( first_prime_after<2>::value == 3 );
    CHECK( first_prime_after<3>::value == 5 );
    CHECK( first_prime_after<4>::value == 5 );
    CHECK( first_prime_after<5>::value == 7 );
    CHECK( first_prime_after<6>::value == 7 );
    CHECK( first_prime_after<7>::value == 11 );
    CHECK( first_prime_after<8>::value == 11 );
    CHECK( first_prime_after<9>::value == 11 );
    CHECK( first_prime_after<10>::value == 11 );
    CHECK( first_prime_after<11>::value == 13 );
    CHECK( first_prime_after<12>::value == 13 );
    CHECK( first_prime_after<13>::value == 17 );
    CHECK( first_prime_after<14>::value == 17 );
    CHECK( first_prime_after<15>::value == 17 );
    CHECK( first_prime_after<16>::value == 17 );
}

TEST_CASE( "Metaprogrammed nth prime", "[math][metaprogramming]" ) {
    using math::prime_list::nth_prime;
    CHECK( nth_prime<0>::value == 2 );
    CHECK( nth_prime<1>::value == 3 );
    CHECK( nth_prime<2>::value == 5 );
    CHECK( nth_prime<3>::value == 7 );
    CHECK( nth_prime<4>::value == 11 );
    CHECK( nth_prime<5>::value == 13 );
    CHECK( nth_prime<6>::value == 17 );
}
