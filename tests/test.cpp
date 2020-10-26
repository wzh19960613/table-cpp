#include <gtest/gtest.h>

#include "Table.hpp"

using namespace uvwee;

namespace {
TEST(Main, test) {
  auto t = Table::headedMDTable(3, 4);
  t.columnInfos[0].alignRight();
  t.columnInfos[2].alignLeft();
  t.rows[0].cellsAppend("abc", "def", "ghi");
  t.rows[2].cellsAppend(1234567890, "", 543.54332, "sadfasdfasdfas");
  (t.rows[3].set(0) << "left").alignLeft().fillWith('+');
  (t.rows[3].set(2) << "right").alignRight().fillWith('-');
  (t.rows[3].set(3) << "center").alignCenter().fillWith(',');
  t.rows[4].cellsAllAppend("m1");
  t.newRow().cellsAppend("m2", "d3", "a4", "b5", "e5");
  t.updateColumnSizes();
  t.align = Align::center;
  std::cout << t;
}
}  // namespace
