#include "math/primitive_root.hpp"
#include <algorithm>
#include <catch.hpp>

std::vector<int> all_primitive_roots_sorted( int p, int a ) {
    auto vec = math::all_primitive_roots_modulo_p( p, a );
    std::sort( vec.begin(), vec.end() );
    return vec;
}

TEST_CASE( "Is primitive root modulo p", "[math]" ) {
    auto factors = math::factor::factor(16);
    CHECK_FALSE( math::is_primitive_root_modulo_p( 0, 17, factors ) );
    CHECK_FALSE( math::is_primitive_root_modulo_p( 1, 17, factors ) );
    CHECK_FALSE( math::is_primitive_root_modulo_p( 2, 17, factors ) );
    CHECK      ( math::is_primitive_root_modulo_p( 3, 17, factors ) );
    CHECK_FALSE( math::is_primitive_root_modulo_p( 4, 17, factors ) );
    CHECK      ( math::is_primitive_root_modulo_p( 5, 17, factors ) );
    CHECK      ( math::is_primitive_root_modulo_p( 6, 17, factors ) );
    CHECK      ( math::is_primitive_root_modulo_p( 7, 17, factors ) );
    CHECK_FALSE( math::is_primitive_root_modulo_p( 8, 17, factors ) );
    CHECK_FALSE( math::is_primitive_root_modulo_p( 9, 17, factors ) );
    CHECK      ( math::is_primitive_root_modulo_p( 10, 17, factors ) );
    CHECK      ( math::is_primitive_root_modulo_p( 11, 17, factors ) );
    CHECK      ( math::is_primitive_root_modulo_p( 12, 17, factors ) );
    CHECK_FALSE( math::is_primitive_root_modulo_p( 13, 17, factors ) );
    CHECK      ( math::is_primitive_root_modulo_p( 14, 17, factors ) );
    CHECK_FALSE( math::is_primitive_root_modulo_p( 15, 17, factors ) );
    CHECK_FALSE( math::is_primitive_root_modulo_p( 16, 17, factors ) );
}

TEST_CASE( "Primitive roots modulo p", "[math]" ) {
    CHECK( math::primitive_root_modulo_p( 2 ) == 1 );
    CHECK( math::primitive_root_modulo_p( 3 ) == 2 );
    CHECK( math::primitive_root_modulo_p( 5 ) == 2 );
    CHECK( math::primitive_root_modulo_p( 7 ) == 3 );
    CHECK( math::primitive_root_modulo_p( 11 ) == 2 );
    CHECK( math::primitive_root_modulo_p( 13 ) == 2 );
    CHECK( math::primitive_root_modulo_p( 17 ) == 3 );
    CHECK( math::primitive_root_modulo_p( 23 ) == 5 );
    CHECK( math::primitive_root_modulo_p( 41 ) == 6 );
    CHECK( math::primitive_root_modulo_p( 47 ) == 5 );
    CHECK( math::primitive_root_modulo_p( 59 ) == 2 );
    CHECK( math::primitive_root_modulo_p( 67 ) == 2 );
    CHECK( math::primitive_root_modulo_p( 71 ) == 7 );
    CHECK( math::primitive_root_modulo_p( 191 ) == 19 );
}

TEST_CASE( "All primitive roots modulo p", "[math]" ) {
    std::vector<int> roots3 = {2};
    std::vector<int> roots5 = {2, 3};
    std::vector<int> roots17 = {3, 5, 6, 7, 10, 11, 12, 14};
    std::vector<int> roots53 = {2, 3, 5, 8, 12, 14, 18, 19, 20,
        21, 22, 26, 27, 31, 32, 33, 34, 35, 39, 41, 45, 48, 50, 51};
    std::vector<int> roots71 = {7, 11, 13, 21, 22, 28, 31, 33, 35,
        42, 44, 47, 52, 53, 55, 56, 59, 61, 62, 63, 65, 67, 68, 69};

    CHECK( all_primitive_roots_sorted( 3, 2 ) == roots3 );
    CHECK( all_primitive_roots_sorted( 5, 2 ) == roots5 );
    CHECK( all_primitive_roots_sorted( 5, 3 ) == roots5 );
    for( auto a : roots17 )
        CHECK( all_primitive_roots_sorted( 17, a ) == roots17 );
    for( auto a : roots53 )
        CHECK( all_primitive_roots_sorted( 53, a ) == roots53 );
    for( auto a : roots71 )
        CHECK( all_primitive_roots_sorted( 71, a ) == roots71 );
}
