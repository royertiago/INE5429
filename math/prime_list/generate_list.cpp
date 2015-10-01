/* This program generates the .cpp that contains the prime list.
 *
 * When this program is executed,
 * it computes the prime list
 * and prints it to stdout.
 * The makefile in this directory takes care of redirecting it to list.cpp.
 */

#include <stdio.h>
#include "math/prime_list/list.h"

namespace {
    const char pre[] =
"#include\"list.h\"\n"
"namespace math {\n"
"    namespace prime_list {\n"
"        const int p[] = {\n"
;
    const char post[] =
"}; // math::prime_list::p\n"
"\n"
"        const int last = p[size-1];\n"
"    } // namespace prime_list\n"
"}// namespace math\n"
;

    // List of prime numbers.
    int p[math::prime_list::size];
} // anonymous namespace

void print() {
    using math::prime_list::size;
    printf( pre );
    int i = 0;
    for( ; i < size; ) {
        for( int k = 0; k < 8 && i < size; k++, i++ )
            printf( "% 9d,", p[i] );
        printf( "\n" );
    }
    printf( post );
}

bool is_prime( int prime ) {
    for( int k = 0; p[k] * p[k] <= prime; k++ )
        if( prime % p[k] == 0 )
            return false;

    return true;
}

void generate() {
    p[0] = 2;
    p[1] = 3;
    for( int i = 2; i < math::prime_list::size; i++ ) {
        int next;
        for( next = p[i-1] + 2 ; !is_prime(next); next += 2 )
            ; // nop
        p[i] = next;
    }
}

int main() {
    generate();
    print();
    return 0;
}
