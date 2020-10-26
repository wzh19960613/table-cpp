#pragma once

#include <gtest/gtest.h>

#include <iostream>

#include "Table.hpp"

template <typename T1, typename T2>
inline bool uvwee_gtest_test_vec(const T1& v1, const T2& v2, const char* n1,
                                 const char* n2) {
  if (v1.size() == v2.size()) {
    std::vector<size_t> ids;
    for (size_t i = 0; i < v1.size(); ++i)
      if (v1[i] != v2[i]) ids.push_back(i);
    if (!ids.size()) return true;
    auto t = uvwee::Table::headedMDTable(0, 3);
    t.columnInfos[0].alignRight();
    t.rows[0].cellsAppend("Index", n1, n2);
    for (auto&& i : ids)
      t.newRow().cellsAppend(i, testing::PrintToString(v1[i]),
                             testing::PrintToString(v2[i]));
    t.updateColumnSizes();
    std::cout << t << std::endl
              << "Total " << ids.size() << " differences." << std::endl;
  } else
    std::cout << "Size of " << n1 << "(" << v1.size() << ") is different with "
              << n2 << "(" << v2.size() << ")!" << std::endl;
  return false;
}

#define TEST_VEC(v1, v2) uvwee_gtest_test_vec(v1, v2, #v1, #v2)

#define ASSERT_VEC(v1, v2) \
  if (!TEST_VEC(v1, v2)) FAIL()

#define EXPECT_VEC(v1, v2) \
  if (!TEST_VEC(v1, v2)) ADD_FAILURE()