#pragma once

#include <iomanip>
#include <optional>
#include <sstream>
#include <string>

#include "Align.hpp"

namespace uvwee {

class Cell {
 public:
  std::optional<Align> align = {};
  std::optional<char> fillChar = {};
  virtual size_t size() const noexcept = 0;
  virtual std::ostream& output(std::ostream& os) const = 0;
  std::ostream& output(std::ostream& os, const size_t& minWidth,
                       Align defaultAlign = Align::left,
                       const char& defaultFillChar = ' ') {
    auto& a = align.has_value() ? align.value() : defaultAlign;
    auto& c = fillChar.has_value() ? fillChar.value() : defaultFillChar;
    auto fn = [&](auto& os, const size_t& size, const char& fillChar,
                  const size_t& minWidth) -> auto& {
      if (minWidth <= size) return size ? output(os) : os;
      auto c = os.fill();
      os << std::setfill(fillChar) << std::setw((int)minWidth);
      return (size ? output(os) : os << fillChar) << std::setfill(c);
    };
    auto s = size();
    if (a != Align::center)
      return fn(os << (a == Align::left ? std::left : std::right), s, c,
                minWidth);
    if (minWidth <= s || !s) return fn(os, s, c, minWidth);
    auto l = (minWidth - s) / 2, r = minWidth - s - l;
    return fn(output(fn(os, 0, c, l)), 0, c, r);
  }
  virtual ~Cell(){};
  Cell& alignLeft() {
    align = Align::left;
    return *this;
  }
  Cell& alignRight() {
    align = Align::right;
    return *this;
  }
  Cell& alignCenter() {
    align = Align::center;
    return *this;
  }
  Cell& alignInherited() {
    align = {};
    return *this;
  }
  Cell& fillWith(const char& c) {
    fillChar = c;
    return *this;
  }
};

/// stringstream Cell
class SSCell : public Cell, public std::stringstream {
 public:
  size_t size() const noexcept override {
    if (fail()) return 0;
    return rdbuf()->pubseekoff(0, std::ios_base::cur, std::ios_base::out);
  }
  std::ostream& output(std::ostream& os) const override { return os << str(); }
};

/// string Cell
class SCell : public Cell, public std::string {
 public:
  using std::string::size;
  std::ostream& output(std::ostream& os) const override { return os << *this; }
};

}  // namespace uvwee

inline std::ostream& operator<<(std::ostream& os, const uvwee::Cell& cell) {
  return cell.output(os);
}

template <class T>
inline uvwee::SSCell& operator<<(uvwee::SSCell& os, const T&& info) {
  os << info;
  return os;
}

template <class T>
inline uvwee::SSCell& operator<<(uvwee::SSCell& os, const T* info) {
  static_cast<std::stringstream&>(os) << info;
  return os;
}