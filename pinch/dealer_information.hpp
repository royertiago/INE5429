#ifndef PINCH_DEALER_INFORMATION_HPP
#define PINCH_DEALER_INFORMATION_HPP

/* Class responsible for managingg the list of shares.
 * This information should be kept secret by the dealer.
 *
 * File format:
 * First, the last_id, in text form.
 * Then, the list of all shares,
 * separated by newline.
 */

#include <iostream>
#include <vector>
#include "pinch/shares.hpp"

namespace pinch {

    template< typename T >
    struct dealer_information {
        std::vector< share<T> > valid_shares;
        int last_id = 0;
    };

    template< typename T >
    std::ostream & operator<<( std::ostream & os, const dealer_information<T> & shares ) {
        os << shares.last_id << '\n';
        for( const share<T> & share : shares.valid_shares )
            os << share << '\n';
        return os;
    }

    template< typename T >
    std::istream & operator>>( std::istream & is, dealer_information<T> & shares ) {
        is >> shares.last_id;
        share<T> s;
        while( is >> s )
            shares.valid_shares.push_back( s );
        is.clear(); // TODO: Maybe this is not a good idea
        return is;
    }

}
#endif // PINCH_DEALER_INFORMATION_HPP
