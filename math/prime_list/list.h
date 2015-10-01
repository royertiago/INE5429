#ifndef MATH_PRIME_LIST_LIST_H
#define MATH_PRIME_LIST_LIST_H

/* List of the first primes,
 * generated through metaprogramming.
 *
 * The prime list is in another translation unit,
 * to avoid the template instantiation cost.
 */

namespace math {
namespace prime_list {
    // Size of the list.
    constexpr int size = 1 << 20;

    /* The actual list.
     * For example,
     * p[0] == 2,
     * p[1] == 3, etc.
     */
    extern const int p[size];

    // Last prime in the list.
    extern const int last;

}} // namespace math::prime_list

#endif // MATH_PRIME_LIST_LIST_H
