#ifndef AUTOSWEEP_CELL_H_
#define AUTOSWEEP_CELL_H_

const uint8_t CELL_UNKNOWN = 255;
const uint8_t CELL_FLAG = 254;

struct Cell {
  int row, col;
  uint8_t value;

  bool IsFlag() const { return value == CELL_FLAG; }
  bool IsUnknown() const { return value == CELL_UNKNOWN; }
  bool IsNumber() const { return value >= 0 && value <= 9; }
  bool IsKnown() const { return IsNumber() || IsFlag(); }

  char ToChar() const {
    switch (value) {
    case CELL_UNKNOWN:
      return '-';
    case CELL_FLAG:
      return 'f';
    case 0:
      return '0';
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    }

    return 'e';
  }

  bool operator==(const Cell &other) const {
    return (row == other.row && col == other.col && value == other.value);
  }
};

namespace std {

template <> struct hash<Cell> {
  std::size_t operator()(const Cell &cell) const {
    using std::hash;
    using std::size_t;
    using std::string;

    return ((hash<int>()(cell.row) ^ (hash<int>()(cell.col) << 1)) >> 1) ^
           (hash<uint8_t>()(cell.value) << 1);
  }
};

} // namespace std

#endif // AUTOSWEEP_CELL_H_
