#ifndef MATH_PRIMITIVE_ROOT_HPP
#define MATH_PRIMITIVE_ROOT_HPP

#include "math/algo.hpp"
#include "math/factor.hpp"

namespace math {
    /* Returns the smallest primitive root modulo p,
     * assuming p is prime,
     * or 0 if no primitive root could be found.
     */
    template< typename T >
    T primitive_root_modulo_p( T p ) {
        /* 2 is the only number for which 1 is a primitive root.
         * Since 1 is also the only primitive root modulo 2,
         * the algorithm below would fail for p == 2
         * if we did not treat it specially.
         */
        if( p == T(2) )
            return T(1);

        T phi = p - 1;
        std::vector<T> exponents;
        for( auto pair : math::factor::factor( phi ) )
            exponents.push_back( phi / pair.first );
        /* It can be shown that a number m is a primitive root modulo p
         * if and only if, for every factor f of p-1, m^((p-1)/f) != 1 (mod p).
         * Thus, we need to raise every candidate to (p-1)/f
         * for every factor f of p-1.
         * The vector exponents just precompute this list.
         */

        T power;
        for( T candidate(2); candidate < p; candidate++ ) {
            for( const auto & exponent : exponents )
                if( math::pow_mod( candidate, exponent, p ) == 1 )
                    // The candidate failed the test.
                    goto next_candidate;

            // Found a primitive root!
            return candidate;

        next_candidate:; // This label functions as a 'continue' command.
        }

        // No primitive roots found; this will never happen for prime p.
        return T(0);
    }

    /* Given a primitive root a modulo a prime p,
     * this algorithm generates all primitive roots modulo p,
     * in no specified order.
     */
    template< typename T >
    std::vector< T > all_primitive_roots_modulo_p( T p, T a ) {
        std::vector< T > primitive_roots = {a};
        T phi = p-1;
        for( T m(2); m < p; m++ ) {
            /* It can be shown that, if a is a primitive root modulo p,
             * then a^m mod p is another primitive root
             * if and only if m is coprime with phi(p) == p-1.
             *
             * Since a is a primitive root,
             * every number modulo p will be a^m for some m,
             * so this algorithm correctly returns every primitive root modulo p.
             */
            if( math::gcd( m, phi ) == T(1) )
                primitive_roots.push_back( math::pow_mod( a, m, p ) );
        }

        return primitive_roots;
    }

} // namespace math

#endif // MATH_PRIMITIVE_ROOT_HPP
