/* /dev/urandom-like interface to the xorshift random number generator.
 * (The output format is suited for the dieharder test suite.)
 */
#include <iostream>
#include <gmpxx.h>
#include "random/xorshift.hpp"

union undefined_behavior {
    unsigned num;
    char str[4];
};

int main() {
    rng::xorshift rng;

    // This kludge invokes undefined behavior, but well... it works and it's short.
    undefined_behavior ub;

    while( true ) {
        ub.num = rng();
        std::cout.write( ub.str, 4 );
    }
}
