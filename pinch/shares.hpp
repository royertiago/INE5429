#ifndef PINCH_SHARES_HPP
#define PINCH_SHARES_HPP

/* Data structure that holds a share for a Pinch secret sharing scheme.
 *
 * File format:
 * The id and the share are written in text form,
 * separated by a space.
 */

#include <iostream>

namespace pinch {

    template< typename T >
    struct share {
        int id;
        T share;
    };

    template< typename T >
    std::ostream & operator<<( std::ostream & os, const share<T>& s ) {
        return os << s.id << ' ' << s.share;
    }

    template< typename T >
    std::istream & operator>>( std::istream & is, share<T>& s ) {
        return is >> s.id >> s.share;
    }
}

#endif // PINCH_SHARES_HPP
