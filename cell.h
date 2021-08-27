#ifndef AUTOSWEEP_CELL_H_
#define AUTOSWEEP_CELL_H_

const uint8_t CELL_UNKNOWN = 255;
const uint8_t CELL_FLAG = 254;

struct Cell {
  int Row, Col;
  uint8_t Value;

  bool IsFlag() const { return Value == CELL_FLAG; }
  bool IsUnknown() const { return Value == CELL_UNKNOWN; }
  bool IsKnown() const {
    return (Value >= 0 && Value <= 9) || Value == CELL_FLAG;
  }

  char ToChar() {
    if (Value == CELL_UNKNOWN)
      return '-';
    if (Value == CELL_FLAG)
      return 'f';
    if (Value == 0)
      return '0';
    if (Value == 1)
      return '1';
    if (Value == 2)
      return '2';
    if (Value == 3)
      return '3';
    if (Value == 4)
      return '4';
    if (Value == 5)
      return '5';
    if (Value == 6)
      return '6';
    if (Value == 7)
      return '7';
    if (Value == 8)
      return '8';
    if (Value == 9)
      return '9';

    return 'e';
  }

  bool operator==(const Cell &other) const {
    return (Row == other.Row && Col == other.Col && Value == other.Value);
  }
};

namespace std {

template <> struct hash<Cell> {
  std::size_t operator()(const Cell &cell) const {
    using std::hash;
    using std::size_t;
    using std::string;

    return ((hash<int>()(cell.Row) ^ (hash<int>()(cell.Col) << 1)) >> 1) ^
           (hash<uint8_t>()(cell.Value) << 1);
  }
};

} // namespace std

#endif // AUTOSWEEP_CELL_H_
