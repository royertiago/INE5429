/* Interface for math::factor::trial_division,
 * similar to the command-line utility 'factor'.
 */

#include <iostream>
#include <gmpxx.h>
#include "math/factor.hpp"

int main( int argc, char ** argv ) {
    mpz_class number;
    if( argc != 2 || gmp_sscanf( argv[1], "%Zd", number.get_mpz_t() ) != 1 ) {
        std::cerr << "Usage: " << argv[0] << " [number to be factored]\n";
        return 1;
    }

    std::cout << number << ':';
    for( auto pair : math::factor::factor( number ) )
        for( int i = 0; i < pair.second; i++ )
            std::cout << ' ' << pair.first;

    std::cout << std::endl;
    return 0;
}
