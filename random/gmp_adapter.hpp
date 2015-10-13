#ifndef RANDOM_GMP_ADAPTER_HPP
#define RANDOM_GMP_ADAPTER_HPP

/* Facilities for using arbitrary random number generators
 * to produce arbitrarily-sized random numbers.
 */

#include <stdio.h>
#include <gmpxx.h>

namespace rng {
    /* Returns a mpz_class with the specified number of bits,
     * using the given random number generator.
     */
    template< typename RNG >
    mpz_class gmp_generate( RNG & rng, std::uint32_t number_of_bits );

// Implementation

    template< typename RNG >
    mpz_class gmp_generate( RNG & rng, std::uint32_t number_of_bits ) {
        if( number_of_bits == 0 )
            return mpz_class( 0 );

        /* The GMP binary format has a 4-byte header,
         * that contains a big-endian integer that says the number of bytes,
         * and then the bytes of the number, also in big-endian order.
         *
         * To generate a number with the specified number of bits,
         * we will first populate the bytes using the given RNG,
         * and then adjust the higher byte so that the returned number
         * has the exact required number of bits.
         */
        std::uint32_t number_of_bytes = (number_of_bits + 7)/8;
        unsigned bits_last_byte = (number_of_bits - 1) % 8 + 1;

        /* The buffer we will write to will be kept as static,
         * so that sucessive invocations of this method does not have
         * the memory allocation overhead. */
        static unsigned char * buf = 0;
        static unsigned buf_size = 0;
        if( 4 + number_of_bytes > buf_size ) {
            delete buf;
            buf = new unsigned char[4 + number_of_bytes];
            buf_size = 4 + number_of_bytes;
        }

        // First, write the size, in guaranteed big-endian order
        std::uint32_t size = number_of_bytes;
        for( int i = 3; i >= 0; i-- ) {
            buf[i] = size % 0x100;
            size /= 0x100;
        }

        // These two values are used to minimize the number of calls to rng().
        constexpr unsigned random_size = sizeof(rng());
        auto random_number = rng();

        // Populate the buffer with random numbers
        int j = 0;
        for( int i = 4; i < 4 + number_of_bytes; i++ ) {
            buf[i] = random_number % 0x100;
            random_number /= 0x100;
            j++;
            if( j == random_size ) {
                j = 0;
                random_number = rng();
            }
        }

        // Adjust the highest byte
        unsigned mask = (1u << bits_last_byte) - 1;
        unsigned lower_limit = mask >> 1;
        /* mask is a bit mask with exactly bits_last_byte ones.
         * lower_limit is the smallest value that will yield
         * the required number of bits in the last byte.
         */
        buf[4] = buf[4] & mask;
        while( buf[4] <= lower_limit ) {
            buf[4] = random_number & mask;
            random_number /= 0x100;
            j++;
            if( j == random_size ) {
                j = 0;
                random_number = rng();
            }
        }

        // Convert the buffer to a mpz_class
        FILE * memory = fmemopen( buf, 4 + number_of_bytes, "r" );
        mpz_class ret;
        mpz_inp_raw(ret.get_mpz_t(), memory);
        fclose( memory );

        return ret;
    }

} // namespace rng

#endif // RANDOM_GMP_ADAPTER_HPP
