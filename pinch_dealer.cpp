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
"--add <share>\n"
"    Add an user to the database and stores its share in the given file name.\n"
"    This file should be sent through a secure channel to the user,\n"
"    and be kept private.\n"
"    Multiple calls to --add may be done simultaneously.\n"
"\n"
"--remove <N>\n"
"    Remove the share of the user with the given ID.\n"
"    The ID is avaliable as the first value of the file that stores the share,\n"
"    that was generated using the option --add.\n"
"    To guarantee that the specified user is not able to access the secret,\n"
"    the noticeboard must be regenerated with another secret.\n"
"\n"
"--noticeboard <file>\n"
"    Chooses the noticeboard file and generate it.\n"
"    --secret and --threshold must also be set.\n"
"\n"
"--secret <N>\n"
"    Sets the secret to be shared.\n"
"    See the option --noticeboard.\n"
"\n"
"--threshold <N>\n"
"    Chooses the threshold of the scheme.\n"
"    See the option --noticeboard.\n"
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
#include "random/xorshift.hpp"

namespace command_line {
    bool generate_share_database = false;
    std::string share_database;
    mpz_class prime_number;
    mpz_class generator;

    std::vector< std::string > added_users;
    std::vector< int > removed_users;

    std::string noticeboard_file;
    mpz_class secret;
    int threshold;

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
            if( arg == "--add" ) {
                added_users.push_back( args.next() );
                continue;
            }
            if( arg == "--remove" ) {
                int n;
                args.range( 1 ) >> n;
                removed_users.push_back( n );
                continue;
            }
            if( arg == "--noticeboard" ) {
                noticeboard_file = args.next();
                continue;
            }
            if( arg == "--secret" ) {
                args.range( 1 ) >> secret;
                continue;
            }
            if( arg == "--threshold" ) {
                args.range( 1 ) >> threshold;
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
    rng::xorshift rng;

    // Set up database and the file
    pinch::dealer_information<mpz_class> database;
    if( command_line::generate_share_database ) {
        database.prime = command_line::prime_number;
        database.generator = command_line::generator;
    }
    else {
        // Database exists.
        std::ifstream file( command_line::share_database );
        file >> database;
    }

    // Add all the needed users
    for( std::string share_filename : command_line::added_users ) {
        std::ofstream share( share_filename );
        share << database.new_share( rng ) << '\n';
    }

    // Remove the requested IDs
    for( int id : command_line::removed_users )
        if( !database.remove_share(id) )
            std::cerr << "Error: user " << id << " not found in the database.\n";

    // Generate the noticeboard
    if( command_line::noticeboard_file != "" ) {
        std::ofstream noticeboard_file( command_line::noticeboard_file );
        pinch::noticeboard<mpz_class> board = database.generate_noticeboard(
            command_line::secret, command_line::threshold, rng
        );
        noticeboard_file << board;
    }

    /* Write database back to the file
     * We must open a new fstream instead of reusing the one used to read the database
     * because standard file streams support truncation only on opening,
     * and the new database could be smaller than the original
     * due to user removal.
     */
    std::ofstream file( command_line::share_database, std::ios::trunc );
    file << database;

    return 0;
}
