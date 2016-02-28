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
 * If the number is found not to be prime and the 'witness' pointer is not null,
 * the found Fermat witness to the compositeness of the number will be stored there.
 * Otherwise, the number is left untouched.
 *
 * Note that even if the number of trials is enormous,
 * there are some non-prime numbers (the Charmicael numbers)
 * that will pass the Fermat test.
 */
template< typename RNG >
bool fermat( mpz_class number, RNG& rng, int trials, mpz_class * witness = nullptr );

// Implementation

template< typename RNG >
bool fermat( mpz_class number, RNG& rng, int trials, mpz_class * witness ) {
    mpz_class power, witness_candidate;

    int bits = mpz_sizeinbase( number.get_mpz_t(), 2 );
    mpz_class number_minus_one = number - 1;

    while( trials-- ) {
        // Generate witness candidate in range [1, n-1]
        witness_candidate = rng::gmp_generate( rng, bits );
        witness_candidate %= number_minus_one;
        witness_candidate += 1;

        // Test witness
        power = math::pow_mod( witness_candidate, number_minus_one, number );

        if( power != 1 ) {
            /* Failed this trial. By Fermat's little theorem,
             * we are sure this number is not prime. */
            if( witness ) *witness = witness_candidate;
            return false;
        }
    }
    // Passed all trials
    return true;
}

}} // namespace math::primality

#endif // MATH_PRIMALITY_HPP
