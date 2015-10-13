#ifndef MATH_GENERATE_PRIMES_HPP
#define MATH_GENERATE_PRIMES_HPP

#include "random/gmp_adapter.hpp"
#include "math/primality.hpp"

namespace math {

    template< typename RNG >
    mpz_class generate_prime_number( RNG & rng, std::uint32_t bits, int trials ) {
        mpz_class number;
        do {
            number = rng::gmp_generate( rng, bits );
        } while( !math::primality::fermat( number, rng, trials ) );

        return number;
    }

}

#endif // MATH_GENERATE_PRIMES_HPP
