#ifndef PROTOCOLS_RSA_HPP
#define PROTOCOLS_RSA_HPP

/* Algorithms and data structures to support the RSA cryptography algorithm.
 */

#include "math/algo.hpp"

namespace rsa {
    /* This structure represents the public key of the algorithm.
     * This data may be shared with everyone.
     */
    template< typename T >
    struct public_key {
        T b, n;

        // Encrypts the given number.
        T encrypt( T ) const;
    };

    /* This structure represents the private key of the algorithm.
     * This data should be kept private.
     */
    template< typename T >
    struct private_key {
        T a, n;

        // Decrypts the given number.
        T decrypt( T ) const;
    };

    /* Constructs the public key for the algorithm.
     * p and q must be primes.
     * b may be any number coprime with both p-1 and q-1.
     */
    template< typename T >
    public_key<T> build_public_key( T p, T q, T b );

    /* Constructs the corresponding private key.
     * p, q and b must be the same for the function build_public_key.
     */
    template< typename T >
    private_key<T> build_private_key( T p, T q, T b );

    // Implementation of template code
    template< typename T >
    public_key<T> build_public_key( T p, T q, T b ) {
        return { b, p * q };
    }

    template< typename T >
    private_key<T> build_private_key( T p, T q, T b ) {
        return { math::modular_inverse( b, T( (p-1) * (q-1) ) ), p * q };
    }

    template< typename T >
    T public_key<T>::encrypt( T x ) const {
        return math::pow_mod( x, b, n );
    }

    template< typename T >
    T private_key<T>::decrypt( T y ) const {
        return math::pow_mod( y, a, n );
    }
}

#endif // PROTOCOLS_RSA_HPP
