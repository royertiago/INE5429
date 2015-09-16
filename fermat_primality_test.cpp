/* Interface to the fermat primalty test.
 */
#include <iostream>
#include "random/xorshift.hpp"
#include "prime/fermat.hpp"

int main( int argc, char ** argv ) {
    if( argc != 3 ) {
        std::cerr << "Usage: " << argv[0] << " [number] [trials]\n";
        return 1;
    }

    mpz_class number( argv[1] );
    int trials;
    sscanf( argv[2], "%d", &trials );

    rng::xorshift rng;
    if( fermat_probably_prime( number, rng, trials ) )
        std::cout << "Number " << number << " passed all " << trials << " trials.\n";
    else
        std::cout << "Number " << number << " failed Fermat test.\n";

    return 0;
}
