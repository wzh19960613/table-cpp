#pragma once

#include <gtest/gtest.h>

#include <iostream>

#include "Table.hpp"

template <class>
struct sfinae_true : std::true_type {};
template <class T>
static auto test_insertion(int)
    -> sfinae_true<decltype(std::cout << std::declval<T>())>;
template <class T>
static auto test_insertion(long) -> std::false_type;
template <class T>
struct is_printable : decltype(test_insertion<T>(0)) {};

template <typename = std::enable_if_t<!is_printable<char32_t>::value>>
inline std::ostream& operator<<(std::ostream& os, const char32_t c) {
  return os << (uint32_t)c;
}

template <typename T1, typename T2>
inline testing::AssertionResult uvwee_gtest_test_vec(const T1& v1, const T2& v2,
                                                     const char* n1,
                                                     const char* n2) {
  if (v1.size() == v2.size()) {
    std::vector<size_t> ids;
    for (size_t i = 0; i < v1.size(); ++i)
      if (v1[i] != v2[i]) ids.push_back(i);
    if (!ids.size()) return testing::AssertionSuccess();
    auto t = uvwee::Table::headedMDTable(0, 3);
    t.columnInfos[0].alignRight();
    t.rows[0].cellsAppend("Index", n1, n2);
    for (auto&& i : ids)
      t.newRow().cellsAppend(i, testing::PrintToString(v1[i]),
                             testing::PrintToString(v2[i]));
    t.updateColumnSizes();
    std::stringstream ss;
    ss << t;
    return testing::AssertionFailure()
           << ss.str() << std::endl
           << "Total " << ids.size() << " differences.";
  }
  return testing::AssertionFailure()
         << "Size of " << n1 << ": " << v1.size() << " is different with " << n2
         << ": " << v2.size() << " !";
}

#define TEST_VEC(v1, v2) uvwee_gtest_test_vec(v1, v2, #v1, #v2)

#define ASSERT_VEC(v1, v2) GTEST_ASSERT_(TEST_VEC(v1, v2), GTEST_FATAL_FAILURE_)

#define EXPECT_VEC(v1, v2) \
  GTEST_ASSERT_(TEST_VEC(v1, v2), GTEST_NONFATAL_FAILURE_)
