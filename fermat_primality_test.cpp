/* Interface to the fermat primalty test.
 */
#include <iostream>
#include "random/xorshift.hpp"
#include "math/primality.hpp"

int main( int argc, char ** argv ) {
    if( argc != 3 ) {
        std::cerr << "Usage: " << argv[0] << " [number] [trials]\n";
        return 1;
    }

    mpz_class number( argv[1] );
    int trials;
    sscanf( argv[2], "%d", &trials );

    rng::xorshift rng;
    mpz_class witness;

    if( math::primality::fermat( number, rng, trials, &witness ) )
        std::cout << "Number " << number << " passed all " << trials << " trials.\n";
    else
        std::cout << "Number " << number << " failed Fermat test.\n"
            << "Witness: " << witness << "\n";

    return 0;
}
