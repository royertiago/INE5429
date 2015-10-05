#include "protocols/diffie_hellman.hpp"
#include <gmpxx.h>
#include "random/xorshift.hpp"
#include <catch.hpp>

TEST_CASE( "Diffie-Hellman key exchange", "[protocol]" ) {
    rng::xorshift rng;

    // Both parts use the same prime number and generator.
    protocol::diffie_hellman<> alice( 2017, 5 );
    protocol::diffie_hellman<> bob( 2017, 5 );

    // Both parts generate a private random number.
    alice.generate_private_number( rng );
    bob.generate_private_number( rng );

    // Both parts send their public numbers to each other.
    auto alice_public = alice.get_public_number();
    auto bob_public = bob.get_public_number();
    alice.set_partner_public_number( bob_public );
    bob.set_partner_public_number( alice_public );

    // Now, they share a common number.
    CHECK( alice.get_common_secret() == bob.get_common_secret() );
}
