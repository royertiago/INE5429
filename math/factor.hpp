#ifndef MATH_FACTOR_HPP
#define MATH_FACTOR_HPP

/* Implementation of factorization algorithms.
 */
#include <vector>
#include <utility>

namespace math { namespace factor {
    /* Every function of this namespace returns the list of factors
     * of the given number.
     * More precisely,
     * a list of factors is a list of pairs (p_i, alpha_i)
     * such that every p_i is a prime number, p_i < p_{i+1}
     * and the product of all p_i^alpha_i equals n.
     *
     * Every prime is of type T to allow for use of libraries like GMP.
     * Every power is of type int because 2^{2^31},
     * the smallest value whose exponent is not representable in an integer,
     * requires two billion bits to be stored.
     * If you need to factor numbers with billions of digits,
     * you are lost anyway.
     */
    template< typename T >
    using factor_list = std::vector< std::pair<T, int> >;

    /* Attempt to factor n using every odd number as a candidate.
     * This algorithm is almost as slow as brute-force.
     */
    template< typename T >
    factor_list<T> trial_division( T n ) {
        factor_list<T> factors;

        // Try dividing by two first
        if( n % T(2) == 0 ) {
            factors.push_back( {T(2), 0} );
            while( n % T(2) == 0 ) {
                n >>= 1;
                factors.back().second++;
            }
        }

        // Now, try every odd number
        for( T divisor(3); divisor * divisor <= n; divisor += 2 ) {
            if( n % divisor == 0 ) {
                factors.push_back( {divisor, 0} );
                while( n % divisor == 0 ) {
                    factors.back().second++;
                    n /= divisor;
                }
            }
        }

        // Finally, add the number itself
        if( n != T(1) )
            factors.push_back( {n, 1} );

        return factors;
    }

}} // namespace math::factor

#endif // MATH_FACTOR_HPP
