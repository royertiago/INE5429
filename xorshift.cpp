namespace command_line {
    const char help_message[] =
" [options]\n"
"Simplistic command-line interface to the xorshift random number generator.\n"
"This program can output both a list of numbers in ASCII form\n"
"or produce a binary stream of random bits, much like /dev/urandom.\n"
"\n"
"Options:\n"
"--text\n"
"    Sets the output to be in text form.\n"
"    This is the default.\n"
"\n"
"--numbers <N>\n"
"--count <N>\n"
"--words <N>\n"
"    Generate the desired amount of random numbers.\n"
"    All random numbers will be between 0 and 2^31-1.\n"
"    Choose a negative value to generate numbers indefinitely.\n"
"    Default: 10\n"
"\n"
"--binary\n"
"    Sets the output to binary and number of words to be unlimited,\n"
"    thus functioning more or less like a 'cat /dev/urandom'.\n"
"\n"
"--x <N>\n"
"--y <N>\n"
"--z <N>\n"
"--w <N>\n"
"    Sets specific values for the internal state bits of the generator.\n"
"    Default: x and z gets the lowest 32 bits of current time,\n"
"    y and w gets the highest 32 bits.\n"
"\n"
"--help\n"
"    Displays this help and quit.\n"
;
}

#include <iostream>
#include <gmpxx.h>
#include "cmdline/args.hpp"
#include "random/xorshift.hpp"

rng::xorshift global_rng;
// This object may be edited during command line parsing.

namespace command_line {
    int n = 10;
    bool binary = false;

    void parse( cmdline::args && args ) {
        while( args.size() > 0 ) {
            std::string arg = args.next();
            if( arg == "--text" ) {
                binary = false;
                continue;
            }
            if( arg == "--number" || arg == "--words" || arg == "--count" ) {
                args >> n;
                continue;
            }
            if( arg == "--binary" ) {
                binary = true;
                n = -1;
                continue;
            }
            if( arg == "--x" ) {
                args >> global_rng.x;
                continue;
            }
            if( arg == "--y" ) {
                args >> global_rng.y;
                continue;
            }
            if( arg == "--z" ) {
                args >> global_rng.z;
                continue;
            }
            if( arg == "--w" ) {
                args >> global_rng.w;
                continue;
            }
            if( arg == "--help" ) {
                std::cout << "Usage: " << args.program_name() << help_message;
                std::exit( 0 );
            }
            std::cerr << args.program_name() << ": Unknown option " << arg << '\n';
            std::exit(1);
        }
    }
} // namespace command_line

union undefined_behavior {
    unsigned num;
    char str[4];
};

int main( int argc, char ** argv ) {
    command_line::parse( cmdline::args(argc, argv) );

    // This kludge invokes undefined behavior, but well... it works and it's short.
    undefined_behavior ub;

    for( int i = 0; command_line::n < 0 || i < command_line::n; i++ ) {
        ub.num = global_rng();
        if( command_line::binary )
            std::cout.write( ub.str, 4 );
        else
            std::cout << ub.num << '\n';
    }
}
