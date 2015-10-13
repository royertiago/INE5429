#ifndef PROTOCOLS_DIFFIE_HELLMAN_HPP
#define PROTOCOLS_DIFFIE_HELLMAN_HPP

/* Implementation of Diffie-Hellman-Merkle's key exchange protocol.
 */

#include <gmpxx.h>
#include "math/algo.hpp"
#include "random/gmp_adapter.hpp"

namespace protocol {
    /* The entire protocol is encapsulated within the following class.
     * The constructor requires both a large prime number
     * and a primitive root of that prime number.
     * Once constructed,
     * the same object can be used for several key exchanges.
     */
    template< typename T = mpz_class >
    class diffie_hellman {
        T prime, primitive_root;
        T private_number;
        T public_number;
        T partner_public_number;
        T common_secret;

    public:
        diffie_hellman( T prime, T primitive_root ):
            prime( prime ),
            primitive_root( primitive_root )
        {}

        /* Generate our private number,
         * that will be used to construct the common secret number.
         */
        template< typename RNG >
        void generate_private_number( RNG & rng );

        /* Returns the public number that must be sent to our partner.
         */
        T get_public_number() const;

        /* Informs the public number of our partner.
         */
        void set_partner_public_number( T t );

        /* Returns the secret number which we share with our partner.
         */
        T get_common_secret() const;
    };

// Implementation

    template< typename T > template< typename RNG >
    void diffie_hellman<T>::generate_private_number( RNG & rng ) {
        // TODO: Make this GMP-independent
        int bits = mpz_sizeinbase( prime.get_mpz_t(), 2 );
        private_number = rng::gmp_generate( rng, bits );
        private_number %= prime;

        public_number = math::pow_mod( primitive_root, private_number, prime );
    }

    template< typename T >
    T diffie_hellman<T>::get_public_number() const {
        return public_number;
    }

    template< typename T >
    void diffie_hellman<T>::set_partner_public_number( T t ) {
        partner_public_number = t;
        common_secret = math::pow_mod( t, private_number, prime );
    }

    template< typename T >
    T diffie_hellman<T>::get_common_secret() const {
        return common_secret;
    }

} // namespace protocol

#endif // PROTOCOLS_DIFFIE_HELLMAN_HPP
