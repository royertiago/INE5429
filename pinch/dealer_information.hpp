#ifndef PINCH_DEALER_INFORMATION_HPP
#define PINCH_DEALER_INFORMATION_HPP

/* Class responsible for managingg the list of shares.
 * This information should be kept secret by the dealer.
 *
 * File format:
 * In text form, the following numbers: last_id, prime, generator,
 * separated by whitespace.
 * Then, the list of all shares,
 * separated by newline.
 */

#include <iostream>
#include <vector>
#include "math/primitive_root.hpp"
#include "math/set.hpp"
#include "random/gmp_adapter.hpp"
#include "pinch/noticeboard.hpp"
#include "pinch/shares.hpp"

namespace pinch {

    template< typename T >
    struct dealer_information {
        std::vector< share<T> > valid_shares;
        int last_id = 0;
        T prime;
        T generator;

        /* Generates a new share for the list.
         */
        template< typename RNG >
        share<T> new_share( RNG & );

        /* Remove the share with the specified identifier.
         * To ensure that the specified user is not able to access the secret,
         * the noticeboard must be regenerated with another secret
         * after running this function.
         *
         * Returns true if something was removed, and false otherwise.
         */
        bool remove_share( int id );

        /* Generate a noticeboard for the given secret and the given threshold.
         * That is, every group with at least threshold members
         * will be able to reconstruct the given secret
         * using the information in the noticeboard.
         *
         * The RNG will be used to create the generators.
         */
        template< typename RNG >
        noticeboard<T> generate_noticeboard( T secret, int threshold, RNG & rng ) const;

    };

    template< typename T >
    std::ostream & operator<<( std::ostream & os, const dealer_information<T> & shares ) {
        os << shares.last_id << ' ' << shares.prime << ' ' << shares.generator << '\n';
        for( const share<T> & share : shares.valid_shares )
            os << share << '\n';
        return os;
    }

    template< typename T >
    std::istream & operator>>( std::istream & is, dealer_information<T> & shares ) {
        is >> shares.last_id >> shares.prime >> shares.generator;
        share<T> s;
        while( is >> s )
            shares.valid_shares.push_back( s );
        is.clear(); // TODO: Maybe this is not a good idea
        return is;
    }

    template< typename T >
    template< typename RNG >
    share<T> dealer_information<T>::new_share( RNG & rng ) {
        // TODO: Make this GMP-independent
        int bits = mpz_sizeinbase( prime.get_mpz_t(), 2 );
        auto number = rng::gmp_generate( rng, bits );
        share<T> new_share{++last_id, T(number % prime)};
        valid_shares.push_back( new_share );
        return new_share;
    }

    template< typename T >
    bool dealer_information<T>::remove_share( int id ) {
        for( int i = 0; i < valid_shares.size(); i++ )
            if( valid_shares[i].id == id ) {
                valid_shares.erase( valid_shares.begin() + i );
                return true;
            }
        return false;
    }

    template< typename T >
    template< typename RNG >
    noticeboard<T> dealer_information<T>::generate_noticeboard(
        T secret,
        int threshold,
        RNG & rng
    ) const {
        noticeboard<T> board;
        board.generator = generator;
        board.prime_modulo = prime;

        std::vector<int> indexes;
        for( int i = 0; i < valid_shares.size(); i++ )
            indexes.push_back( i );

        for( auto group_indexes: math::sorted_subsets( indexes, threshold ) ) {
            group_data<T> data;
            data.group_generator =
                math::random_primitive_root_modulo_p( prime, generator, rng );

            T power = 1; // Power that g_X must be raised to compute V_X.
            for( auto index : group_indexes ) {
                power *= valid_shares[index].share;
                data.group.push_back( valid_shares[index].id );
            }

            T V_X = math::pow_mod( data.group_generator, power, prime );
            T f_V_X = math::pow_mod( generator, V_X, prime );
            data.group_value = (secret - f_V_X + prime) % prime;

            board.groups.push_back( data );
        }

        return board;
    }

}
#endif // PINCH_DEALER_INFORMATION_HPP
