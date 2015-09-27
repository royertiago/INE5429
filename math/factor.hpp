/* Implementation of the trial division factorization algorithm.
 */
#include <vector>
#include <utility>

namespace math { namespace factor {

    /* Returns a list of pairs <p_i, alpha_i>
     * such that every p_i is a prime number,
     * p_i < p_{i+1}
     * and the product of all p_i^alph_i equals n.
     *
     * Note that this algorithm is almost as slow as brute-force.
     */
    template< typename T >
    std::vector< std::pair<T, int> > trial_division( T n ) {
        std::vector< std::pair<T, int> > factors;

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
