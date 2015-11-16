#ifndef MATH_SET_HPP
#define MATH_SET_HPP

/* Tools for manipulating sets.
 */

#include <algorithm>
#include <vector>

namespace math {

    /* Returns all the subsets of the given size
     * for the given set.
     *
     * This algorithm assumes that size >= set.size().
     */
    template< typename T >
    std::vector< std::vector<T> > subsets( std::vector<T> set, int size ) {
        if( size == 0 )
            // There is only one subset with size zero.
            return {{}};

        std::vector< std::vector<T> > ret_val;
        for( int i = 0; i + size <= set.size(); i++ ) {
            std::vector<T> remainder( set.begin() + i + 1, set.end() );
            auto smaller_subsets = subsets( remainder, size - 1 );
            for( auto subset : smaller_subsets ) {
                subset.push_back( set[i] );
                ret_val.push_back( subset );
            }
        }
        return ret_val;
    }

    /* Returns all the subsets of the given size,
     * but sorted.
     */
    template< typename T >
    std::vector< std::vector<T> > sorted_subsets( std::vector<T> set, int size ) {
        auto r = subsets( set, size );
        for( auto & a : r )
            std::sort( a.begin(), a.end() );
        std::sort( r.begin(), r.end() );
        return r;
    }
}
#endif // MATH_SET_HPP
