/* Program that finds a primitive root modulo p,
 * assuming p is a prime.
 */

#include <iostream>
#include <gmpxx.h>
#include "math/primitive_root.hpp"

int main( int argc, char ** argv ) {
    mpz_class prime;
    if( argc != 2 || gmp_sscanf( argv[1], "%Zd", prime.get_mpz_t() ) != 1 ) {
        std::cerr << "Usage: " << argv[0] << " [number]\n";
        return 1;
    }

    mpz_class root = math::primitive_root_modulo_p( prime );
    if( root != 0 ) {
        std::cout << root << " is a primitive root modulo " << prime << std::endl;
        return 0;
    }

    std::cout << "No primitive roots found for " << prime << std::endl;
    return 1;
}
