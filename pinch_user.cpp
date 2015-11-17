namespace command_line {
    const char help_message[] =
" <private share> [options]\n"
"Manages a single user share.\n"
"The options determite what to do.\n"
"\n"
"Every file generated by this program may be exchanged freely,\n"
"without the need for a secure channel.\n"
"\n"
"Options:\n"
"--reconstruct <noticeboard> <passing_message> <random_number> <N>...\n"
"    Starts the secret reconstruction process.\n"
"    noticeboard is the board which holds the needed public information.\n"
"    passing_message is a file that will be generated\n"
"    to hold the data to coordinate the reconstruction.\n"
"    random_number is a file that will be generated with a random number\n"
"    generated only at the beginning of the reconstruction process.\n"
"\n"
"    The file <noticeboard> is public and <passing_message> may be sent freely.\n"
"    The random number must be kept private; it will be used to finish\n"
"    the secret reconstruction.\n"
"\n"
"    The last values are the ID's of the users\n"
"    that will help reconstruct the secret.\n"
"\n"
"--join <passing_message>\n"
"    Join your share to the given reconstruction effort.\n"
"\n"
"--finish-reconstruction <passing_message> <random_number>\n"
"    After every user had joined its share to the \"reconstruction message\",\n"
"    this command uses the random number to finish the reconstruction\n"
"    and prints to stdout the secret.\n"
"\n"
"--help\n"
"    Displays this help and quit.\n"
;
} // namespace command_line

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <gmpxx.h>
#include "cmdline/args.hpp"
#include "pinch/shares.hpp"
#include "pinch/noticeboard.hpp"
#include "random/xorshift.hpp"

namespace command_line {
    std::string share;
    std::string noticeboard;
    std::string message;
    std::string random_file;
    std::vector<int> users;

    void parse( cmdline::args && args ) {
        while( args.size() > 0 ) {
            std::string arg = args.next();
            if( arg == "--reconstruct" ) {
                noticeboard = args.next();
                message = args.next();
                random_file = args.next();
                while( args.size() > 0 ) {
                    int user;
                    args >> user;
                    users.push_back( user );
                }
                break;
            }
            if( arg == "--join" ) {
                message = args.next();
                continue;
            }
            if( arg == "--finish-reconstruction" ) {
                message = args.next();
                random_file = args.next();
                continue;
            }
            if( arg == "--help" ) {
                std::cout << "Usage: " << args.program_name() << help_message;
                std::exit( 0 );
            }
            if( share == "" )
                share = arg;
            else {
                std::cerr << "Error: Specifying the share twice (\"" << share
                    << "\" and \"" << arg << "\")\n";
                std::exit( 1 );
            }
        }
        while( args.size() > 0 )
            std::cerr << "Ignored argument " << args.next();
    }
}

int main( int argc, char ** argv ) {
    command_line::parse( cmdline::args( argc, argv ) );
    rng::xorshift rng;

    if( command_line::noticeboard != "" ) {
        pinch::noticeboard<mpz_class> board;
        pinch::share<mpz_class> share;
        {
            std::ifstream noticeboard_file( command_line::noticeboard );
            std::ifstream share_file( command_line::share );
            noticeboard_file >> board;
            share_file >> share;
        }
        auto pair = share.start_reconstruction( board, command_line::users, rng );
        {
            std::ofstream message_file( command_line::message );
            std::ofstream random_file( command_line::random_file );
            message_file << pair.first;
            random_file << pair.second;
        }
        if( pair.first.remaining_ids.size() > 0 )
            std::cout << "Generated " << command_line::message << ".\n"
                << "Send this file to user " << pair.first.remaining_ids[0]
                << " to continue.\n";
        else
            std::cout << "Generated " << command_line::message << ".\n"
                << "Return this file back to the starter to finish.\n";
    }
    else if( command_line::random_file == "" ) {
        pinch::share<mpz_class> share;
        pinch::message<mpz_class> message;
        {
            std::ifstream share_file( command_line::share );
            std::ifstream message_file( command_line::message );
            share_file >> share;
            message_file >> message;
        }
        share.join( message );
        {
            std::ofstream message_file( command_line::message );
            message_file << message;
        }
        if( message.remaining_ids.size() > 0 )
            std::cout << "Joined your share to " << command_line::message << ".\n"
                << "Send this file to user " << message.remaining_ids[0]
                << " to continue.\n";
        else
            std::cout << "Joined your share to " << command_line::message << ".\n"
                << "Return this file back to the starter to finish.\n";
    }
    else {
        pinch::message<mpz_class> message;
        pinch::private_nonce<mpz_class> nonce;
        {
            std::ifstream message_file( command_line::message );
            std::ifstream nonce_file( command_line::random_file );
            message_file >> message;
            nonce_file >> nonce;
        }
        std::cout << "Secret: " << nonce.reconstruct( message ) << '\n';
    }

    return 0;
}
