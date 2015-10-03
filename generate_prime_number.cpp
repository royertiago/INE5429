/* Program that generates a prime number of the given size.
 *
 * This program uses Fermat's test for primalty, with 30 trials,
 * and the xorshift algorithm for random number generation.
 */

#include <cstdio>
#include <iostream>
#include "random/xorshift.hpp"
#include "math/primality.hpp"

int main( int argc, char ** argv ) {
    int digits;
    if( argc != 2 || std::sscanf( argv[1], "%d", &digits ) != 1 ) {
        std::cerr << "Usage: " << argv[0] << " [number of bytes]\n";
        return 1;
    }

    rng::xorshift rng;
    int count = 0;
    mpz_class number;
    printf( "Digits = %d\n", digits );

    do {
        number = rng::gmp_generate( rng, digits );
        count++;
        std::cout << "Trying " << number << '\n';
    }
    while( !fermat_probably_prime( number, rng, 30 ) );

    std::cout << "Found prime number " << number
        << " after " << count << " trials.\n";
    return 0;
}
