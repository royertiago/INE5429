/* Program that finds a primitive root modulo p,
 * assuming p is a prime.
 */

#include <iostream>
#include <gmpxx.h>
#include "math/factor.hpp"
#include "math/algo.hpp"

int main( int argc, char ** argv ) {
    mpz_class prime;
    if( argc != 2 || gmp_sscanf( argv[1], "%Zd", prime.get_mpz_t() ) != 1 ) {
        std::cerr << "Usage: " << argv[0] << " [number]\n";
        return 1;
    }

    mpz_class phi = prime - 1;
    std::vector< mpz_class > exponents;
    for( auto pair : math::factor::trial_division( phi ) )
        exponents.push_back( phi / pair.first );

    /* It can be shown that a number m is a primitive root modulo p
     * if and only if, for every factor f of p-1, m^((p-1)/f) != 1 (mod p).
     * Thus, we need to raise every candidate to (p-1)/f
     * for every factor f of p-1.
     * The vector exponents just precompute this list.
     */

    mpz_class power;
    for( mpz_class candidate = 2; candidate < prime; candidate++ ) {
        for( const auto & exponent : exponents )
            if( math::pow_mod( candidate, exponent, prime ) == 1 )
                // The candidate failed the test.
                goto next_candidate;

        // Found a primitive root!
        std::cout << candidate << " is a primitive root modulo " << prime << std::endl;
        return 0;

next_candidate:;
    }

    std::cout << "No primitive roots found for " << prime << std::endl;
    return 1;
}
