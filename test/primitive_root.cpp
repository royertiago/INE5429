#include "math/primitive_root.hpp"
#include <catch.hpp>

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
