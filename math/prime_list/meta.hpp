#ifndef MATH_PRIME_LIST_META_HPP
#define MATH_PRIME_LIST_META_HPP

/* Functions used to aid the creation of a precomputed list of prime numbers,
 * using template metaprogramming.
 */

#include <type_traits>

namespace math {
namespace prime_list {

    /* Returns true if N is a prime number,
     * and 0 otherwise.
     *
     * Inherits from std::integral_constant<bool>;
     * assumes N is a nonnegative number.
     */
    template< int N >
    struct is_prime;

    /* Returns the (N-1)th prime number.
     * That is,
     * nth_prime<0>::value == 2,
     * nth_prime<1>::value == 3,
     * and so on.
     *
     * Inherits from std::integral_constant<int>;
     * assumes N is a nonnegative number.
     */
    template< int N >
    struct nth_prime;

    /* Returns the first prime number after N.
     *
     * Inherits from std::integral_constant<int>;
     * assumes N is a nonnegative number.
     */
    template< int N >
    struct first_prime_after;


// Implementation


// Implementation of is_prime

    /* Hardcode the special cases for 0 and 1
     * (which are not prime altough they cannot be divided by any number
     * smaller than them)
     * and the for first prime number.
     */
    template<> struct is_prime<0> : public std::false_type {};
    template<> struct is_prime<1> : public std::false_type {};
    template<> struct is_prime<2> : public std::true_type {};

    /* For general N, we will use iteration to search for factors of N.
     * The class iterated_primalty_test will be responsible for this search.
     *
     * Template parameters:
     *  PrimeCandidate - number which we are trying to find prime factors.
     *  Index - Index of the next prime number we need to test if it divides
     *      PrimeCandidate. This value tells that every prime p_i for 0 <= i < Index
     *      does not divide our prime candidate.
     *  stop_iteration - Becomes true when p_index * p_index > PrimeCandidate.
     *  was_divisible - Becomes true when p_{index - 1} divides PrimeCandidate.
     *
     * Every instantiation of this template will define a nested type named 'type'
     * which will be either std::true_type or std::false_type.
     */
    template< int PrimeCandidate, int Index, bool stop_iteration, bool was_divisible >
    struct iterated_primality_test;

    /* First special case:
     * if was_divisible became true somewhere,
     * our candidate must be rejected.
     */
    template< int PrimeCandidate, int Index, bool stop_iteration >
    struct iterated_primality_test< PrimeCandidate, Index, stop_iteration, true > {
        typedef std::false_type type;
    };

    /* Second special case:
     * if the iteration extrapolated the square root of the prime number,
     * AND it was not divisible by the previous prime number tested,
     * then the number must be prime.
     */
    template< int PrimeCandidate, int Index >
    struct iterated_primality_test< PrimeCandidate, Index, true, false > {
        typedef std::true_type type;
    };

    /* Now, the recursive implementation of the structure.
     *
     * The factorization attempt will take place in the parameter was_divisible.
     * Since we already have dealt with the case when was_divisible becomes true,
     * we can safely call the template recursively with and incremented index.
     *
     * Note we use the template nth_prime to iterate through the prime list.
     * We just require instantiation of nth_prime<Index>
     * until the square of nth_prime<Index>::value exceeds PrimeCandidate;
     * so, as nth_prime<Index> require instantiation of is_prime
     * until we actually reach the nth prime,
     * this does not causes an infinite loop.
     */
    template< int PrimeCandidate, int Index, bool stop_iteration, bool was_divisible >
    struct iterated_primality_test {
        typedef typename iterated_primality_test<
            PrimeCandidate,
            Index+1,
            (nth_prime<Index>::value * nth_prime<Index>::value > PrimeCandidate),
            PrimeCandidate % nth_prime<Index>::value == 0
        >::type type;
    };

    /* Finally, use the structure to implement is_prime.
     * As is_prime always inherits from std::integral_constant,
     * we must actually extend from iterated_primalty_test::type.
     */
    template< int N > struct is_prime : public
        iterated_primality_test<N, 0, false, false>::type {};

// Implementation of nth_prime

    /* Using first_prime_after,
     * this one is actually simple to implement.
     */
    template<> struct nth_prime<0> : public std::integral_constant<int, 2> {};
    template< int N > struct nth_prime :
        public first_prime_after< nth_prime<N-1>::value > {};


// Implementation of first_prime_after

    /* Again, we will implement iteration through template recursion.
     * The template will simply call is_prime for every number in sucession
     * until a prime number is found.
     *
     * Template parameters:
     *  N - The number in question.
     *  stop - If true, means that N is the desired prime number we are searching for.
     *      If false, means the prime desired prime number is larger than N
     *      and we need another iteration.
     *
     * The member type "type" is a std::integral_constant with the adequated value.
     *
     * In searching for the first prime number p after a number N,
     * we need to call is_prime<T> for every T such that N < T <= p.
     * Since testing primality does not require first_prime_after
     * for numbers p larger than N, we do not fall in an infinite loop.
     */
    template< int N, bool stop >
    struct iterated_prime_search {
        typedef typename iterated_prime_search< N+1, is_prime<N+1>::value >::type type;
    };

    template< int N >
    struct iterated_prime_search<N, true> {
        typedef std::integral_constant<int, N> type;
    };

    // Now, use the recursion above to implement first_prime_after.
    template< int N >
    struct first_prime_after : iterated_prime_search<N, false>::type {};

}} // namespace math::prime_list

#endif // MATH_PRIME_LIST_META_HPP
