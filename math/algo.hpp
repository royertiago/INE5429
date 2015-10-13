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

    /* Returns the greatest common divisor of a and b,
     * using an iterative version of the euclidean algorithm.
     */
    template< typename T >
    T gcd( T a, T b ) {
        T tmp;
        while( b != 0 ) {
            tmp = a % b;
            a = b;
            b = tmp;
        }
        return a;
    }

    /* Data for the Extended Euclid's algorithm.
     */
    template< typename T >
    struct euclid_data {
        T x;
        T y;
        T gcd; // The greatest common divisor
    };

    template< typename T >
    euclid_data<T> extended_euclid( T a, T b ) {
        T xx = T(1), xy = T(0), yx = T(0), yy = T(1);
        while( b != 0 ) {
            T nxy = xx - xy*(a/b);
            xx = xy;
            xy = nxy;

            T nyy = yx - yy*(a/b);
            yx = yy;
            yy = nyy;

            T nb = a % b;
            a = b;
            b = nb;
        }
        return {xx, yx, a};
    }

    /* Computes the inverse of a modulo n.
     * This algorithm assumes gcd(a, n) == 1.
     */
    template< typename T >
    T modular_inverse( T a, T n ) {
        auto d = extended_euclid( a, n );
        return (d.x + n) % n;
    }

} // namespace math

#endif // MATH_ALGO_HPP
