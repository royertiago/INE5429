#!/bin/bash
if (( $# != 1 )); then
    echo "Usage: $0 <number of bytes>"
    echo "Search for a weak prime with the given number of bytes."
    echo "(A weak prime is a prime 'p' such that"
    echo "the number p-1 can be factored by ./factor under 10 seconds.)"
    exit 0;
fi

bytes=$1

while true; do
    number=$(./generate_prime_number $bytes)
    number_minus_one=$(bc <<< "$number - 1" | tr -d '\n\\')

    echo Trying $number...

    timeout 10 ./factor $number_minus_one
    if (( $? == 0 )); then
        echo Found weak prime $number
        break;
    fi
done
