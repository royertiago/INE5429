namespace command_line {
    const char help_message[] =
" [options] <prime number>\n"
"Finds a primitive root modulo the given number,\n"
"assuming it is prime.\n"
"\n"
"Currently, the algorithm returns the smallest primitive root.\n"
"\n"
"Options:\n"
"--all\n"
"    Generate all primitive roots modulo p instead of just one.\n"
"\n"
"--sort\n"
"    Sort the list of primitive roots in ascending order.\n"
"    Default: no particular order.\n"
"\n"
"--root <N>\n"
"    Informs the program that this number is a primitive root of that prime.\n"
"    This can be used in conjunction with --all to sped up the generation\n"
"    of the list of primitive numbers.\n"
"\n"
"--help\n"
"    Displays this help and quit.\n"
;
} // namespace command_line

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <gmpxx.h>
#include "cmdline/args.hpp"
#include "math/primitive_root.hpp"

namespace command_line {
    mpz_class prime, root;
    bool generate_all = false;
    bool sort = false;
    bool known_initial_root = false;

    void parse( cmdline::args && args ) {
        while( args.size() > 0 ) {
            std::string arg = args.next();
            if( arg == "--all" ) {
                generate_all = true;
                continue;
            }
            if( arg == "--sort" ) {
                sort = true;
                continue;
            }
            if( arg == "--root" ) {
                args >> root;
                known_initial_root = true;
                continue;
            }
            if( arg == "--help" ) {
                std::cout << "Usage: " << args.program_name() << help_message;
                std::exit( 0 );
            }
            if( gmp_sscanf( arg.c_str(), "%Zd", prime.get_mpz_t() ) != 1 ) {
                std::cerr << args.program_name() << ": Unknown option " << arg << '\n';
                std::exit(1);
            }
        }
    }
}

int main( int argc, char ** argv ) {
    command_line::parse( cmdline::args(argc, argv) );
    if( !command_line::known_initial_root )
        command_line::root = math::primitive_root_modulo_p( command_line::prime );

    if( command_line::root == 0 ) {
        std::cout << "No primitive roots found for " << command_line::prime;
        return 1;
    }

    std::vector<mpz_class> roots = {command_line::root};

    if( command_line::generate_all )
        roots = math::all_primitive_roots_modulo_p(
            command_line::prime, command_line::root
        );
    if( command_line::sort )
        std::sort( roots.begin(), roots.end() );

    for( const auto & root : roots )
        std::cout << root << '\n';

    return 0;
}
