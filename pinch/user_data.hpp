#ifndef PINCH_USER_DATA_HPP
#define PINCH_USER_DATA_HPP

/* Data owned by the user of the Pinch scheme.
 *
 * File structure (message):
 * The group_data, partial_message, prime_modulo and generator
 * are written in one line, separated by whitespace;
 * then, the list of remaining ids is written in the other line.
 */

#include <vector>
#include <iostream>

namespace pinch {

    template< typename T >
    struct message {
        std::vector<int> remaining_ids;
        T group_data;
        T partial_message;
        T prime_modulo;

        T generator; // Generator to the one-way function
    };

    template< typename T >
    struct private_nonce {
        T nonce_inverse;
    };

    template< typename T >
    std::ostream & operator<<( std::ostream & os, const message<T> & m ) {
        os << m.group_data << ' ' << m.partial_message << ' '
            << m.prime_modulo << ' ' << m.generator << '\n';
        const char * sep = "";
        for( int i : m.remaining_ids ) {
            os << sep << i;
            sep = " ";
        }
        return os << '\n';
    }

    template< typename T >
    std::istream & operator>>( std::istream & is, message<T> & m ) {
        is >> m.group_data >> m.partial_message >> m.prime_modulo >> m.generator;
        int i;
        while( is >> i )
            m.remaining_ids.push_back(i);
        is.clear(); // TODO: maybe not a good idea
        return is;
    }

    template< typename T >
    std::ostream & operator<<( std::ostream & os, const private_nonce<T> & nonce ) {
        return os << nonce.nonce_inverse << '\n';
    }

    template< typename T >
    std::istream & operator>>( std::istream & is, private_nonce<T> & nonce ) {
        return is >> nonce.nonce_inverse;
    }
}

#endif // PINCH_USER_DATA_HPP
