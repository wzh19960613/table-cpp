#pragma once

#include <string>
#include <vector>

#include "Cell.hpp"

namespace uvwee {
class ColumnInfos;
class Row : public std::vector<std::unique_ptr<Cell>> {
 public:
  using std::vector<std::unique_ptr<Cell>>::vector;
  template <class T, class... TP>
  T& add(TP&&... args) {
    push_back(std::make_unique<T>(args...));
    return *(T*)back().get();
  }

  template <class T = SSCell, class... TP>
  T& set(const size_t& n, TP&&... args) {
    return *(T*)(operator[](n) = std::move(std::make_unique<T>(args...))).get();
  }

  template <class T = SSCell, class... TP>
  T& get(const size_t& n, TP&&... args) {
    auto p = operator[](n).get();
    return p ? *(T*)p : set(n, args...);
  }

  template <class... TP>
  void rangeCellsAllAppend(size_t from, size_t to, TP&&... args) {
    if (from >= to) return;
    auto s = sizeof...(args), d = to - from;
    while (d >= s) {
      rangeToEndCellsAppend(from, args...);
      from += s;
      d -= s;
    }
    if (d) rangeCellsAppend(from, from + d, args...);
  }

  template <class... TP>
  void rangeToEndCellsAllAppend(size_t from, TP&&... args) {
    rangeCellsAllAppend(from, size(), args...);
  }

  template <class... TP>
  void cellsAllAppend(TP&&... args) {
    rangeCellsAllAppend(0, size(), args...);
  }

  template <class... TP>
  void cellsAppend(TP&&... args) {
    rangeCellsAppend(0, size(), args...);
  }

  template <class T>
  void rangeToEndCellsAppend(size_t at, T&& arg) {
    get(at) << arg;
  }

  template <class T, class... TP>
  void rangeToEndCellsAppend(size_t from, T&& arg, TP&&... args) {
    rangeToEndCellsAppend(from, arg);
    rangeToEndCellsAppend(++from, args...);
  }

  template <class T>
  void rangeCellsAppend(size_t from, size_t to, T&& arg) {
    if (from >= to) return;
    rangeToEndCellsAppend(from, arg);
  }

  template <class T, class... TP>
  void rangeCellsAppend(size_t from, size_t to, T&& arg, TP&&... args) {
    if (from >= to) return;
    rangeToEndCellsAppend(from, arg);
    rangeCellsAppend(++from, to, args...);
  }

  size_t noEmptySize() const {
    for (auto n = size(); n;)
      if (auto& c = operator[](--n); c != nullptr && c->size()) return n + 1;
    return 0;
  }
  std::ostream& output(std::ostream& os, const ColumnInfos& columnInfos,
                       Align defaultAlign = Align::left) const;

  void allFill(const char& c) {
    for (auto i = 0; i < size(); ++i) {
      auto& p = operator[](i);
      if (!p) p = std::move(std::make_unique<SSCell>());
      p->fillChar = c;
    }
  }
};

class Rows : public std::vector<Row> {
 public:
  std::string prefix = "";
  std::string separator = "\n";
  std::string suffix = "";
  size_t notEmptyColumnInfoSize() const {
    size_t n = 0;
    for (auto&& row : *this)
      if (auto&& s = row.noEmptySize(); s > n) n = s;
    return n;
  }
  size_t notEmptySize() const {
    for (auto n = size(); n;)
      if (auto& c = operator[](--n); c.noEmptySize()) return n + 1;
    return 0;
  }
  using std::vector<Row>::resize;
  void resizeColumnInfos(const size_t& columnInfosNum) {
    for (auto&& r : *this) r.resize(columnInfosNum);
  }
  void resize(const size_t& rowsNum, const size_t& columnInfosNum) {
    resize(rowsNum);
    resizeColumnInfos(columnInfosNum);
  }
  void shrink() {
    resize(notEmptySize());
    resizeColumnInfos(notEmptyColumnInfoSize());
  }
  std::ostream& output(std::ostream& os, const ColumnInfos& columnInfos,
                       Align defaultAlign = Align::left) const;
};
}  // namespace uvwee
