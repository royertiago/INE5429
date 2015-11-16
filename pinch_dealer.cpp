namespace command_line {
    const char help_message[] =
" <private share database> [options]\n"
"Manages a dealer's share database.\n"
"The options determite what to do.\n"
"\n"
"Options:\n"
"--prime <N>\n"
"    Choose to generate a new database, with the given prime number\n"
"    being the chosen modulo.\n"
"    You need also to set --generator.\n"
"\n"
"--generator <N>\n"
"    Provides a generator for the corresponding prime.\n"
"    See the option --prime.\n"
"\n"
"--help\n"
"    Displays this help and quit.\n"
;
} // namespace command_line

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <gmpxx.h>
#include "cmdline/args.hpp"
#include "pinch/dealer_information.hpp"

namespace command_line {
    bool generate_share_database = false;
    std::string share_database;
    mpz_class prime_number;
    mpz_class generator;

    std::vector< std::string > added_users;

    void parse( cmdline::args && args ) {
        while( args.size() > 0 ) {
            std::string arg = args.next();
            if( arg == "--prime" ) {
                args.range( 1 ) >> prime_number;
                generate_share_database = true;
                continue;
            }
            if( arg == "--generator" ) {
                args.range( 1 ) >> generator;
                generate_share_database = true;
                continue;
            }
            if( arg == "--help" ) {
                std::cout << "Usage: " << args.program_name() << help_message;
                std::exit( 0 );
            }

            if( share_database == "" )
                share_database = arg;
            else {
                std::cerr << args.program_name() << ": Unknown option " << arg
                    << "\nMaybe you supplied two share databases?\n";
                std::exit(1);
            }
        }
        if( generate_share_database ) {
            if( prime_number == 0 ) {
                std::cerr << "Provide a prime number with the option --prime.\n";
                std::exit(1);
            }
            if( generator == 0 ) {
                std::cerr << "Provide a generator with the option --generator\n";
                std::exit(1);
            }
        }
    }
}

int main( int argc, char ** argv ) {
    command_line::parse( cmdline::args( argc, argv ) );

    pinch::dealer_information<mpz_class> database;
    std::fstream file( command_line::share_database, std::ios::in | std::ios::out );

    if( !command_line::generate_share_database )
        // Database exists.
        file >> database;

    if( command_line::generate_share_database ) {
        database.prime = command_line::prime_number;
        database.generator = command_line::generator;
    }

    // Write database back to the file
    file.clear();
    file.seekg( 0 );
    file << database;

    return 0;
}