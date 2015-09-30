/* Implementation of math/prime_list/list.h.
 */
#include <utility>
#include "math/prime_list/meta.hpp"
#include "list.h"
#include <stdio.h>

namespace {
    template< template <int...> class T, typename Sequence  >
    struct call_with_integer_sequence;

    template< template <int...> class T, int ... Indexes >
    struct call_with_integer_sequence< T, std::integer_sequence<int, Indexes... > > {
        typedef T<Indexes...> type;
    };

    template< int... Indexes >
    struct prime_list_holder_t {
        static const int list[sizeof...(Indexes)];
    };

    template< int ... Indexes >
    const int prime_list_holder_t< Indexes... >::list[sizeof...(Indexes)] = {
        math::prime_list::nth_prime<Indexes>::value...
    };

    using prime_list_holder =
        call_with_integer_sequence<
            ::prime_list_holder_t,
            std::make_integer_sequence<int, math::prime_list::size>
        >::type;

} // anonymous namespace

const int * const math::prime_list::p = prime_list_holder::list;

const int math::prime_list::last = p[math::prime_list::size - 1];
