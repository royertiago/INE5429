#ifndef MATH_PRIMALITY_HPP
#define MATH_PRIMALITY_HPP

#include <gmpxx.h>
#include "math/algo.hpp"
#include "random/gmp_adapter.hpp"

namespace math {
namespace primality {

/* Fermat primality test.
 * Performs the selected number of trials,
 * using the given random number generator.
 *
 * Note that even if the number of trials is enormous,
 * there are some non-prime numbers (the Charmicael numbers)
 * that will pass the Fermat test.
 */
template< typename RNG >
bool fermat( mpz_class number, RNG& rng, int trials );

// Implementation

template< typename RNG >
bool fermat( mpz_class number, RNG& rng, int trials ) {
    mpz_class power, witness_candidate;

    int bytes = (mpz_sizeinbase( number.get_mpz_t(), 2 ) + 7)/8;
    mpz_class number_minus_one = number - 1;

    while( trials-- ) {
        // Generate witness candidate in range [1, n-1]
        witness_candidate = rng::gmp_generate( rng, bytes );
        witness_candidate %= number_minus_one;
        witness_candidate += 1;

        // Test witness
        power = math::pow_mod( witness_candidate, number_minus_one, number );

        if( power != 1 )
            /* Failed this trial. By Fermat's little theorem,
             * we are sure this number is not prime. */
            return false;
    }
    // Passed all trials
    return true;
}

}} // namespace math::primality

#endif // MATH_PRIMALITY_HPP
