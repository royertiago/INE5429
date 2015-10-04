#ifndef MATH_FACTOR_HPP
#define MATH_FACTOR_HPP

/* Implementation of factorization algorithms.
 */
#include <vector>
#include <utility>
#include "math/algo.hpp"
#include "math/prime_list/list.h"
#include "math/primality.hpp"
#include "random/xorshift.hpp"

namespace math { namespace factor {
    /* This is the data type returned by the main function of this module,
     * "factor".
     *
     * A list of factors is a list of pairs (p_i, alpha_i)
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

    /* Returns the factors of the number n.
     * This function coordinates the efforts of the functions below;
     * unless you want to fine-tune the algorithm,
     * this is the only function you will ever need to call.
     *
     * This function uses probabilistic methods
     * to both test primality in intermediate steps
     * and to initialize the algorithms.
     * (Thus the algorithm actually has a small chance of failing.)
     * The parameter rng allows the caller to, at least,
     * control the initial seed used by the algorithm.
     */
    template< typename T, typename RNG = rng::xorshift >
    factor_list<T> factor( T n, RNG rng = rng::xorshift() );

// Specialized functions

    /* Try to divide the given number by small primes first.
     * The number n is divided by any factors found.
     * Returns an ordered list of factors,
     * which are guaranteed to be prime.
     *
     * 'iterations' is the number of small primes the algorithm will use.
     * This algorithm uses the precomputed prime list in math::prime_list,
     * so it assumes that 'iterations' is at most math::prime_list::size.
     */
    template< typename T >
    factor_list<T> trial_division( T & n, int iterations = math::prime_list::size );

// Implementation

    template< typename T >
    factor_list<T> trial_division( T & n, int iterations = math::prime_list::size ) {
        factor_list<T> factors;

        for( int k = 0; k < iterations; k++ ) {
            int divisor = prime_list::p[k];

            if( n % divisor == 0 ) {
                /* Found a prime factor.
                 * This algorithm will never go back to the current divisor again,
                 * so we must try to divide 'n' by 'divisor'
                 * as many times as possible.
                 */
                factors.push_back( {T(divisor), 0} );
                while( n % divisor == 0 ) {
                    factors.back().second++;
                    n /= divisor;
                }
            }

            if( divisor * divisor > n ) {
                /* We fully factored the number.
                 *
                 * If n is different than 1, then it is guaranteed to be prime;
                 * we will add it to the factor list and update it accordingly
                 * (remember n is taken by reference).
                 *
                 * n might become 1 due to the loop above,
                 * which happens in the case that n == p * p.
                 */
                if( n != T(1) ) {
                    factors.push_back( {n, 1} );
                    n = T(1);
                }
                break;
            }
        }
        return factors;
    }

    /* Default function used by Pollard's Rho algorithm.
     */
    template< typename T >
    T pollard_rho_default_function( T n ) {
        return n * n + 1;
    }

    /* Pollard's Rho algorithm,
     * with Brent's cycle detection algorithm.
     *
     * The random number generator is used to choose where to start the algorithm
     * and to test primality in intermediate steps.
     */
    template< typename T, typename RNG = rng::xorshift, typename F = T(T) >
    factor_list<T> pollard_rho(
        T n,
        RNG rng = rng::xorshift(),
        F f = pollard_rho_default_function<T>
    ) {
        factor_list<T> factors;

        if( math::primality::fermat( n, rng, 30 ) ) {
            factors.push_back( {n, 1} );
            return factors;
        }

        T i(1); // Current iteration index.
        T l_i(0); // Value l(i) - 1, the index against which we are comparing to.

        T x_l_i( rng() % n ); // X_{l(i) - 1}, the value against which
        // we will compare to.

        T x_i = f(x_l_i) % n; // X_i; Current value of iteration

        /* We have initialized x_l_i before x_i to guarantee that
         *  x_i = x_1 = f(x_0) = f(x_l_i).
         */

        T d = gcd( T(n + x_i - x_l_i), n );
        /* d is the candidate to a divisor of n.
         * We will iterate until d == n,
         * in which the algorithm have failed.
         */

        while( d != n ) {
            if( d != T(1) ) {
                /* Found nontrivial divisor of n!
                 * Add it to the list and keep factoring.
                 */
                factors.push_back( {d, 1} );
                n /= d;
                x_i %= n;
                x_l_i %= n;

                if( math::primality::fermat( n, rng, 30 ) ) {
                    // No more factoring to do.
                    factors.push_back( {n, 1} );
                    return factors;
                }
            }

            if( 2*l_i + 1 == i ) {
                /* Must reset both l_i and x_l_i. */
                l_i = 2*l_i + 1;
                x_l_i = x_i;
            }

            ++i;
            x_i = f(x_i) % n;
            d = gcd( T(n + x_i - x_l_i), n );
        }

        if( d == n ) {
            /* We failed.
             * TODO: fail more elegantly.
             */
            factors.push_back( {n, 1} );
        }

        return factors;
    }

    // Implementation of factor, using Pollard's rho algorithm directly
    template< typename T, typename RNG >
    factor_list<T> factor( T n, RNG rng ) {
        return pollard_rho( n, rng );
    }

}} // namespace math::factor

#endif // MATH_FACTOR_HPP
