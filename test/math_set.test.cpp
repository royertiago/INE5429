#include "math/set.hpp"
#include <catch.hpp>

TEST_CASE( "math::subsets", "[math]" ) {
    std::vector<int> vec = {1, 2, 3};
    std::vector< std::vector<int> > subsets, ret_subsets;

    subsets = {{}};
    CHECK( math::sorted_subsets(vec, 0) == subsets );

    subsets = {{1}, {2}, {3}};
    CHECK( math::sorted_subsets(vec, 1) == subsets );

    subsets = {{1, 2}, {1, 3}, {2, 3}};
    CHECK( math::sorted_subsets(vec, 2) == subsets );

    subsets = {{1, 2, 3}};
    CHECK( math::sorted_subsets(vec, 3) == subsets );

    vec = {1, 2, 3, 4};
    subsets = {{}};
    CHECK( math::sorted_subsets(vec, 0) == subsets );

    subsets = {{1}, {2}, {3}, {4}};
    CHECK( math::sorted_subsets(vec, 1) == subsets );

    subsets = {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}};
    CHECK( math::sorted_subsets(vec, 2) == subsets );

    subsets = {{1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}};
    CHECK( math::sorted_subsets(vec, 3) == subsets );

    subsets = {{1, 2, 3, 4}};
    CHECK( math::sorted_subsets(vec, 4) == subsets );
}
