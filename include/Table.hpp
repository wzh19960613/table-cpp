#pragma once

#include <iostream>

#include "ColumnInfo.hpp"

namespace uvwee {

class Table {
 public:
  Rows rows;
  ColumnInfos columnInfos;
  std::optional<Align> align = {};

  void resize(const size_t& rowsNum, const size_t& columnInfosNum) {
    rows.resize(rowsNum, columnInfosNum);
    columnInfos.resize(columnInfosNum);
  }

  std::ostream& output(std::ostream& os) const {
    return rows.output(os, columnInfos,
                       align.has_value() ? align.value() : Align::left);
  }

  void shrink() { rows.shrink(); }

  void updateColumnSizes() { columnInfos.updateSizes(rows); }

  std::string str() const {
    std::stringstream ss;
    output(ss);
    return ss.str();
  }

  Row& newRow() { return rows.emplace_back(columnInfos.size()); }

  static Table headedMDTable(const size_t& rowsNum, const size_t& columnNum) {
    Table t;
    t.resize(rowsNum + 2, columnNum);
    t.rows.prefix = "|";
    t.rows.suffix = "|";
    t.columnInfos.prefix = " ";
    t.columnInfos.suffix = " ";
    t.columnInfos.separator = "|";
    t.rows[1].allFill('-');
    return t;
  }
};

}  // namespace uvwee

inline std::ostream& operator<<(std::ostream& os, const uvwee::Table& table) {
  return table.output(os);
}