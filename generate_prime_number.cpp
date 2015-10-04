namespace command_line {
    const char help_message[] =
" [options] <number of bytes>\n"
"Generates a prime number with the chosen number of bytes.\n"
"\n"
"The program will randomly generate numbers with the desired amount of bytes\n"
"(using Xorshift for random number generation),\n"
"and use the Fermat primality test find a prime.\n"
"\n"
"Options:\n"
"--verbose\n"
"    Print the numbers being tested.\n"
"\n"
"--trials <N>\n"
"    Chose the number of trials to Fermat's primality test.\n"
"    Default: 30.\n"
"\n"
"--help\n"
"    Displays this help and quit.\n"
;
} // namespace command_line

#include <iostream>
#include "cmdline/args.hpp"
#include "random/xorshift.hpp"
#include "math/primality.hpp"

namespace command_line {
    bool verbose = false;
    int fermat_trials = 30;
    int bytes;

    void parse( cmdline::args && args ) {
        while( args.size() > 0 ) {
            std::string arg = args.peek();
            if( arg == "--verbose" ) {
                args.shift();
                verbose = true;
                continue;
            }
            if( arg == "--trials" ) {
                args.shift();
                args >> fermat_trials;
                continue;
            }
            if( arg == "--help" ) {
                std::cout << "Usage: " << args.program_name() << help_message;
                std::exit( 0 );
            }
            args >> bytes;
        }
    }
}


int main( int argc, char ** argv ) {
    command_line::parse( cmdline::args(argc, argv) );

    rng::xorshift rng;
    int attempts = 0;
    mpz_class number;

    do {
        number = rng::gmp_generate( rng, command_line::bytes );
        attempts++;
        if( command_line::verbose )
            std::cout << "Trying " << number << '\n';
    }
    while( !math::primality::fermat( number, rng, command_line::fermat_trials ) );

    if( command_line::verbose )
        std::cout << "Found prime number ";

    std::cout << number;

    if( command_line::verbose )
        std::cout << " after " << attempts << " attempts.";

    std::cout << std::endl;
    return 0;
}
