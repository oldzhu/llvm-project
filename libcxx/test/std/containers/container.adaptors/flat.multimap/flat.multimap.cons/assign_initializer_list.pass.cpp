//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_map>

// flat_multimap& operator=(initializer_list<value_type> il);

#include <algorithm>
#include <cassert>
#include <deque>
#include <flat_map>
#include <functional>
#include <ranges>
#include <vector>

#include "MinSequenceContainer.h"
#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"
#include "../helpers.h"

template <class KeyContainer, class ValueContainer>
constexpr void test() {
  using Key   = typename KeyContainer::value_type;
  using Value = typename ValueContainer::value_type;
  using M     = std::flat_multimap<Key, Value, std::less<Key>, KeyContainer, ValueContainer>;
  {
    M m = {{8, 8}, {10, 10}};
    assert(m.size() == 2);
    m = {{3, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1}, {4, 0}, {3, 2}, {5, 0}, {6, 0}, {5, 1}};
    assert(std::ranges::equal(m.keys(), std::vector{{1, 2, 2, 3, 3, 3, 4, 5, 5, 6}}));
    check_possible_values(
        m.values(),
        std::vector<std::vector<Value>>{
            {0},
            {0, 1},
            {0, 1},
            {0, 1, 2},
            {0, 1, 2},
            {0, 1, 2},
            {0},
            {0, 1},
            {0, 1},
            {0},
        });
  }
  {
    M m = {{10, 1}, {8, 1}};
    assert(m.size() == 2);
    m                                    = {{3, 2}};
    std::pair<double, double> expected[] = {{3, 2}};
    assert(std::ranges::equal(m, expected));
  }
}

constexpr bool test() {
  test<std::vector<int>, std::vector<int>>();
  test<std::vector<int>, std::vector<double>>();
#ifndef __cpp_lib_constexpr_deque
  if (!TEST_IS_CONSTANT_EVALUATED)
#endif
  {
    test<std::deque<int>, std::vector<double>>();
  }

  test<MinSequenceContainer<int>, MinSequenceContainer<double>>();
  test<std::vector<int, min_allocator<int>>, std::vector<double, min_allocator<double>>>();
  test<std::vector<int, min_allocator<int>>, std::vector<int, min_allocator<int>>>();

  return true;
}

int main(int, char**) {
  test();
#if TEST_STD_VER >= 26
  static_assert(test());
#endif

  return 0;
}
