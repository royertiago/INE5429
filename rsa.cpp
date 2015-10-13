namespace command_line {
    const char help_message[] =
" [options]\n"
"Encrypt/decrypt numbers with the RSA algorithm.\n"
"\n"
"Options:\n"
"--public <key_file>\n"
"    Chooses the specified file as the public key.\n"
"\n"
"--private <key_file>\n"
"    Chooses the specified file as the private key.\n"
"\n"
"--encrypt\n"
"    Sets the program in encrypting mode;\n"
"    numbers to encrypt are read from stdin,\n"
"    encrypted data written to stdout.\n"
"    The option --public must have been set for this to work.\n"
"    This is the default.\n"
"\n"
"--decrypt\n"
"    Sets the program to decrypting mode;\n"
"    numbers to encrypt are read from stdin,\n"
"    encrypted data written to stdout.\n"
"    The option --private must have been set for this to work.\n"
"\n"
"--gen-key <size>\n"
"    Generate a key pair with the given size.\n"
"    Both --public and --private must have been set for this to work.\n"
"    In this mode, no data is read from stdin or written to stdout.\n"
"\n"
"--help\n"
"    Displays this help and quit.\n"
;
} // namespace command_line

#include <iostream>
#include <fstream>
#include "cmdline/args.hpp"
#include "random/xorshift.hpp"
#include "math/generate_primes.hpp"
#include "protocols/rsa.hpp"

namespace command_line {
    std::string public_key_file, private_key_file;
    bool encrypt = true;

    bool gen_key = false;
    int key_size;

    void parse( cmdline::args && args ) {
        while( args.size() > 0 ) {
            std::string arg = args.next();
            if( arg == "--public" ) {
                public_key_file = args.next();
                continue;
            }
            if( arg == "--private" ) {
                private_key_file = args.next();
                continue;
            }
            if( arg == "--encrypt" ) {
                encrypt = true;
                gen_key = false;
                continue;
            }
            if( arg == "--decrypt" ) {
                encrypt = false;
                gen_key = false;
                continue;
            }
            if( arg == "--gen-key" ) {
                gen_key = true;
                args.range(1) >> key_size;
                continue;
            }
            if( arg == "--help" ) {
                std::cout << "Usage: " << args.program_name() << help_message;
                std::exit( 0 );
            }
            std::cerr << "Unknown option " << arg << '\n';
            std::exit( 1 );
        }
        if( (encrypt || gen_key) && public_key_file == "" ) {
            std::cerr << "--public must be set to encrypt or to generate keys.\n";
            std::exit( 1 );
        }
        if( (!encrypt || gen_key) && private_key_file == "" ) {
            std::cerr << "--private must be set to decrypt or to generate keys.\n";
            std::exit( 1 );
        }
    }
} // namespace command_line

int main( int argc, char ** argv ) {
    command_line::parse( cmdline::args( argc, argv ) );

    if( command_line::gen_key ) {
        std::cout << "Generating key\n";
        std::cout << "Public -> " << command_line::public_key_file << '\n';
        std::ofstream public_key_file( command_line::public_key_file );
        std::ofstream private_key_file( command_line::private_key_file );
        rsa::public_key<mpz_class> public_key;
        rsa::private_key<mpz_class> private_key;

        rng::xorshift rng;
        mpz_class p = math::generate_prime_number(rng, command_line::key_size/2, 30);
        mpz_class q = math::generate_prime_number(rng, (command_line::key_size+1)/2, 30);
        mpz_class b = math::generate_prime_number(rng, 2*command_line::key_size/3, 30);
        // This number b is guaranteed to be coprime with both p and q.
        public_key = rsa::build_public_key(p, q, b);
        private_key = rsa::build_private_key(p, q, b);

        public_key_file << public_key << '\n';
        private_key_file << private_key << '\n';
        std::cout << public_key << '\n';
        std::cout << private_key << '\n';
    }
    else if( command_line::encrypt ) {
        std::fstream file( command_line::public_key_file );
        rsa::public_key<mpz_class> public_key;
        file >> public_key;

        mpz_class number;
        while( std::cin >> number )
            std::cout << public_key.encrypt(number) << '\n';
    } else {
        std::fstream file( command_line::private_key_file );
        rsa::private_key<mpz_class> private_key;
        file >> private_key;

        mpz_class number;
        while( std::cin >> number )
            std::cout << private_key.decrypt(number) << '\n';
    }

    return 0;
}
