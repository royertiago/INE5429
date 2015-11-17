#ifndef PINCH_SHARES_HPP
#define PINCH_SHARES_HPP

/* Data structure that holds a share for a Pinch secret sharing scheme.
 *
 * File format:
 * The id and the share are written in text form,
 * separated by a space.
 */

#include <algorithm>
#include <iostream>
#include <utility> // std::pair
#include "math/algo.hpp"
#include "pinch/user_data.hpp"
#include "pinch/noticeboard.hpp" // TODO: circular inclusion; may cause problems
#include "random/gmp_adapter.hpp"

namespace pinch {

    template< typename T >
    struct noticeboard;

    template< typename T >
    struct share {
        int id;
        T share;

        /* Start the reconstruction of the shares,
         * using the given RNG to generate the secret nonce.
         *
         * The user list must not include this ID.
         */
        template< typename RNG >
        std::pair< message<T>, private_nonce<T> > start_reconstruction(
            const noticeboard<T> & board,
            std::vector< int > users,
            RNG & rng
        ) const;

        /* Join this share to the given message.
         */
        void join( message<T> & ) const;
    };

    template< typename T >
    std::ostream & operator<<( std::ostream & os, const share<T>& s ) {
        return os << s.id << ' ' << s.share;
    }

    template< typename T >
    std::istream & operator>>( std::istream & is, share<T>& s ) {
        return is >> s.id >> s.share;
    }

    template< typename T >
    template< typename RNG >
    std::pair< message<T>, private_nonce<T> > share<T>::start_reconstruction(
        const noticeboard<T> & board,
        std::vector< int > users,
        RNG & rng
    ) const {
        message<T> msg;
        private_nonce<T> nonce_holder;
        // First, generate the random nonce.
        T phi = board.prime_modulo - 1;
        int bits = mpz_sizeinbase( phi.get_mpz_t(), 2 );
        T nonce = rng::gmp_generate( rng, bits );
        while( math::gcd( nonce, phi ) != 1 )
            nonce = rng::gmp_generate( rng, bits );

        nonce_holder.nonce_inverse = math::modular_inverse( nonce, phi );

        // Next, retrieve the public information about the user list.
        msg.remaining_ids = users;
        users.push_back( id );
        std::sort( users.begin(), users.end() );
        group_data<T> data = board.retrieve_data( users );

        msg.group_data = data.group_value;
        msg.prime_modulo = board.prime_modulo;
        msg.generator = board.generator;

        // And now, give our contribution to the partial share.
        msg.partial_message = math::pow_mod(
                data.group_generator,
                T( nonce * this->share ),
                msg.prime_modulo
            );

        return std::make_pair( msg, nonce_holder );
    }

    template< typename T >
    void share<T>::join( message<T> & msg ) const {
        for( int i = 0; i < msg.remaining_ids.size(); i++ )
            if( msg.remaining_ids[i] == id ) {
                msg.partial_message = math::pow_mod(
                        msg.partial_message,
                        this->share,
                        msg.prime_modulo
                    );
                msg.remaining_ids.erase( msg.remaining_ids.begin() + i );
                return;
            }
        throw std::out_of_range( "The share of this id is not on the message's group." );
    }
}

#endif // PINCH_SHARES_HPP
