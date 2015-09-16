#ifndef MATH_ALGO_HPP
#define MATH_ALGO_HPP

namespace math {
    /* Computes t^i mod n.
     * We assume T(1) is the multiplicative identity of T
     * and that U is an integer type in which
     * U % 2 and U >>= 1 are fast operations.
     */
    template< typename T, typename U >
    T pow_mod( T t, U i, T n ) {
        t = t % n;
        T r(1);
        while( i != 0 ) {
            if( i % 2 == 1 )
                r = r * t % n;
            t = t * t % n;
            i >>= 1;
        }
        return r;
    }
} // namespace math

#endif // MATH_ALGO_HPP
