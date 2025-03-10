//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// void reserve(size_type res_arg); // constexpr since C++20

// This test relies on https://llvm.org/PR45368 (841132e) being fixed, which isn't in
// older Apple dylibs.
// XFAIL: using-built-library-before-llvm-12

#include <string>
#include <stdexcept>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"

template <class S>
TEST_CONSTEXPR_CXX20 void
test(typename S::size_type min_cap, typename S::size_type erased_index, typename S::size_type res_arg) {
  S s(min_cap, 'a');
  s.erase(erased_index);
  assert(s.size() == erased_index);
  assert(s.capacity() >= min_cap); // Check that we really have at least this capacity.
  LIBCPP_ASSERT(is_string_asan_correct(s));

#if TEST_STD_VER > 17
  typename S::size_type old_cap = s.capacity();
#endif
  S s0 = s;
  if (res_arg <= s.max_size()) {
    s.reserve(res_arg);
    LIBCPP_ASSERT(s.__invariants());
    assert(s == s0);
    assert(s.capacity() >= res_arg);
    assert(s.capacity() >= s.size());
    LIBCPP_ASSERT(is_string_asan_correct(s));
#if TEST_STD_VER > 17
    assert(s.capacity() >= old_cap); // reserve never shrinks as of P0966 (C++20)
#endif
  }
#ifndef TEST_HAS_NO_EXCEPTIONS
  else if (!TEST_IS_CONSTANT_EVALUATED) {
    try {
      s.reserve(res_arg);
      LIBCPP_ASSERT(s.__invariants());
      assert(false);
    } catch (std::length_error&) {
      assert(res_arg > s.max_size());
    }
  }
#endif
}

template <class S>
TEST_CONSTEXPR_CXX20 void test_string() {
  {
    test<S>(0, 0, 5);
    test<S>(0, 0, 10);
    test<S>(0, 0, 50);
  }
  {
    test<S>(100, 1, 5);
    test<S>(100, 50, 5);
    test<S>(100, 50, 10);
    test<S>(100, 50, 50);
    test<S>(100, 50, 100);
    test<S>(100, 50, 1000);
    test<S>(100, 50, S::npos);
  }

  { // Check that growing twice works as expected
    S str;
    str.reserve(50);
    assert(str.capacity() >= 50);
    size_t old_cap = str.capacity();
    str.reserve(str.capacity() + 1);
    assert(str.capacity() > old_cap);
  }
}

TEST_CONSTEXPR_CXX20 bool test() {
  test_string<std::string>();
#if TEST_STD_VER >= 11
  test_string<std::basic_string<char, std::char_traits<char>, min_allocator<char>>>();
#endif

  return true;
}

int main(int, char**) {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return 0;
}
