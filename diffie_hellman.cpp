/* Interface to Diffie-Hellman's protocol.
 */

#include <cstdio>
#include <gmpxx.h>
#include <iostream>
#include "protocols/diffie_hellman.hpp"
#include "random/xorshift.hpp"

int main( int argc, char ** argv ) {
    if( argc != 3 ) {
        std::cout << "Usage: " << argv[0] << " <prime number> <primitive root>\n";
        return 1;
    }

    mpz_class number, primitive_root;
    gmp_sscanf( argv[1], "%Zd", number.get_mpz_t() );
    gmp_sscanf( argv[2], "%Zd", primitive_root.get_mpz_t() );

    protocol::diffie_hellman<> dh( number, primitive_root );
    rng::xorshift rng;

    dh.generate_private_number( rng );
    std::cout << "Our public number: " << dh.get_public_number() << '\n';
    std::cout << "Input parter's public number: ";

    mpz_class partner_public;
    std::cin >> partner_public;
    dh.set_partner_public_number( partner_public );

    std::cout << "Common secret is " << dh.get_common_secret() << '\n';

    return 0;
}
