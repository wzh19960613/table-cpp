#pragma once

#include <gtest/gtest.h>

#include <iostream>

#include "Table.hpp"

#define TEST_VEC(v1, v2, failFunc)                                         \
  if (v1.size() != v2.size()) {                                            \
    std::cout << "Size of " #v1 "(" << v1.size()                           \
              << ") is different with " #v2 "(" << v2.size() << ")!"       \
              << std::endl;                                                \
    failFunc();                                                            \
  } else {                                                                 \
    std::vector<size_t> ids;                                               \
    for (size_t i = 0; i < v1.size(); ++i)                                 \
      if (v1[i] != v2[i]) ids.push_back(i);                                \
    if (ids.size()) {                                                      \
      auto t = uvwee::Table::headedMDTable(0, 3);                          \
      t.columnInfos[0].alignRight();                                       \
      t.rows[0].cellsAppend("Index", #v1, #v2);                            \
      for (auto &&i : ids) t.newRow().cellsAppend(i, v1[i], v2[i]);        \
      t.updateColumnSizes();                                               \
      std::cout << t << std::endl                                          \
                << "Total " << ids.size() << " differences." << std::endl; \
      failFunc();                                                          \
    }                                                                      \
  }

#define ASSERT_VEC(v1, v2) TEST_VEC(v1, v2, FAIL)

#define EXPECT_VEC(v1, v2) TEST_VEC(v1, v2, ADD_FAILURE)