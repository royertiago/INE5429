#ifndef MATH_PRIMITIVE_ROOT_HPP
#define MATH_PRIMITIVE_ROOT_HPP

#include "math/algo.hpp"
#include "math/factor.hpp"

namespace math {
    /* Returns true if a is a primitive root modulo p,
     * and false otherwise.
     * p is assumed to be a prime;
     * factors should be the factor list of p-1 = phi(p).
     */
    template< typename T >
    bool is_primitive_root_modulo_p( T a, T p, const factor::factor_list<T> & factors ) {
        if( a == 0 )
            return false;

        for( const auto & pair : factors ) {
            /* It can be shown that a is a primitive root modulo p
             * if and only if, for every prime factor f of p-1,
             *  a^((p-1)/f) != 1 (mod p).
             * (The only exception is a == 0, which we dealt above.)
             */
            T exponent = (p-1)/pair.first;
            if( pow_mod( a, exponent, p ) == 1 )
                return false;
        }
        return true;
    }

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

        auto factors = math::factor::factor( T(p-1) );

        for( T candidate(2); candidate < p; candidate++ )
            if( is_primitive_root_modulo_p(candidate, p, factors) )
                return candidate;

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
