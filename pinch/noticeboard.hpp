#ifndef PINCH_NOTICEBOARD_HPP
#define PINCH_NOTICEBOARD_HPP

/* Data structure that holds the noticeboard.
 *
 * Every authorized group X has, in the Pinch scheme,
 * a group generator g_X, a group join V_X
 * and a group value T_X = S - f(V_X).
 * (S is the secret.)
 * V_X is obtained by joining the shares of every user in group X.
 * The value T_X and g_X get published in the noticeboard.
 *
 * The function f is a one-way function. In our case,
 * we have chosen modular exponentiation.
 * Thus, the reconstruction phase needs the join of all group shares
 * (the value V_X) and the parameters for f
 * (the database generator and the prime modulo).
 *
 * File format (group_data):
 * The values group_generator, group_value, group.size(), and the elements in group
 * all appear separated by a single whitespace.
 *
 * File format (noticeboard):
 * The generator and prime_modulo (parameters for f) in a single line,
 * separated by whitespace;
 * then, one group_data per line.
 */

#include <iostream>
#include <vector>

namespace pinch {
    template< typename T >
    struct group_data {
        T group_generator;
        T group_value;
        std::vector<int> group;
    };

    template< typename T >
    struct noticeboard {
        T generator, prime_modulo; // Parameters for the function f
        std::vector< group_data<T> > groups;
    };

    template< typename T >
    std::istream & operator>>( std::istream & is, group_data<T> & data ) {
        int size;
        if( !(is >> data.group_generator >> data.group_value >> size) )
            /* Read/write error.
             * Must return now to avoid calling vector.resize
             * with memory garbage. */
            return is;

        data.group.resize( size );
        for( int i = 0; i < size; i++ )
            is >> data.group[i];
        return is;
    }

    template< typename T >
    std::ostream & operator<<( std::ostream & os, const group_data<T> & data ) {
        os << data.group_generator << ' ' << data.group_value
            << ' ' << data.group.size();
        for( int i = 0; i < data.group.size(); i++ )
            os << ' ' << data.group[i];
        return os;
    }

    template< typename T >
    std::istream & operator>>( std::istream & is, noticeboard<T>& board ) {
        is >> board.generator >> board.prime_modulo;
        group_data<T> data;
        while( is >> data )
            board.groups.push_back( data );

        is.clear(); // TODO: maybe not a good idea
        return is;
    }

    template< typename T >
    std::ostream & operator<<( std::ostream & os, const noticeboard<T>& board ) {
        os << board.generator << ' ' << board.prime_modulo << '\n';
        for( const auto & data : board.groups )
            os << data << '\n';
        return os;
    }
}

#endif // PINCH_NOTICEBOARD_HPP
