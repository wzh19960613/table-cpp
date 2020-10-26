#pragma once

#include <vector>

#include "Row.hpp"

namespace uvwee {
class ColumnInfo {
 public:
  size_t size;
  std::optional<Align> align = {};

  void alignLeft() { align = Align::left; }
  void alignRight() { align = Align::right; }
  void alignCenter() { align = Align::center; }
  void alignInherited() { align = {}; }
};

class ColumnInfos : public std::vector<ColumnInfo> {
 public:
  std::string prefix = "";
  std::string separator = "\t";
  std::string suffix = "";
  size_t defaultWidth = 0;
  using std::vector<ColumnInfo>::resize;

  void resize(size_t n) { resize(n, {defaultWidth}); }

  size_t calcSize(const Rows& rows, const size_t& n, const size_t& minWidth) {
    auto w = minWidth;
    for (auto&& row : rows)
      if (n < row.size())
        if (auto& p = row[n]; p != nullptr)
          if (auto&& s = p->size(); s > w) w = s;
    return w;
  }

  size_t calcSize(const Rows& rows, const size_t& n) {
    return calcSize(rows, n, defaultWidth);
  }

  void updateSize(const Rows& rows, const size_t& n, const size_t& minWidth) {
    operator[](n).size = calcSize(rows, n, minWidth);
  }

  void updateSize(const Rows& rows, const size_t& n) {
    updateSize(rows, n, defaultWidth);
  }

  void updateSizes(const Rows& rows, const size_t& minWidth) {
    for (auto n = 0; n < size(); ++n) updateSize(rows, n, minWidth);
  }

  void updateSizes(const Rows& rows) { updateSizes(rows, defaultWidth); }
};

std::ostream& Row::output(std::ostream& os, const ColumnInfos& columnInfos,
                          Align defaultAlign) const {
  auto s = size();
  if (!s) return os;
  auto outputCell = [&](const size_t& i) -> auto& {
    os << columnInfos.prefix;
    auto a = columnInfos[i].align.has_value() ? columnInfos[i].align.value()
                                              : defaultAlign;
    if (i < s)
      if (auto& p = operator[](i); p != nullptr)
        p->output(os, columnInfos[i].size, a);
        
    return os << columnInfos.suffix;
  };
  size_t i = 0;
  for (size_t e = s - 1; i < e; ++i) outputCell(i) << columnInfos.separator;
  return outputCell(i);
}

std::ostream& Rows::output(std::ostream& os, const ColumnInfos& columnInfos,
                           Align defaultAlign) const {
  if (!size()) return os;
  auto i = begin(), e = --end();
  while (i != e)
    i++->output(os << prefix, columnInfos, defaultAlign) << suffix << separator;
  return i->output(os << prefix, columnInfos, defaultAlign) << suffix;
}

}  // namespace uvwee
