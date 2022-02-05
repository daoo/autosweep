#ifndef AUTOSWEEP_CELL_H_
#define AUTOSWEEP_CELL_H_

#include <functional>
#include <sstream>
#include <stdexcept>

enum CellValue : uint8_t {
  CELL_0 = 0,
  CELL_1,
  CELL_2,
  CELL_3,
  CELL_4,
  CELL_5,
  CELL_6,
  CELL_7,
  CELL_8,
  CELL_9,
  CELL_UNKNOWN,
  CELL_MINE,
  CELL_FLAG
};

struct Cell {
  int row, col;
  CellValue value;

  bool IsFlag() const { return value == CELL_FLAG; }
  bool IsUnknown() const { return value == CELL_UNKNOWN; }
  bool IsKnown() const { return !IsUnknown(); }
  bool IsMine() const { return value == CELL_MINE; }
  bool IsZero() const { return value == 0; }
  bool IsNumber() const { return value >= 1 && value <= 9; }

  static CellValue FromChar(char character) {
    switch (character) {
    case '-':
      return CELL_UNKNOWN;
    case 'f':
      return CELL_FLAG;
    case '0':
      return CELL_0;
    case '1':
      return CELL_1;
    case '2':
      return CELL_2;
    case '3':
      return CELL_3;
    case '4':
      return CELL_4;
    case '5':
      return CELL_5;
    case '6':
      return CELL_6;
    case '7':
      return CELL_7;
    case '8':
      return CELL_8;
    case '9':
      return CELL_9;
    }

    std::stringstream error;
    error << "Character '" << character << "' does not represent a valid cell.";
    throw std::runtime_error(error.str());
  }

  char ToChar() const {
    switch (value) {
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
    case CELL_UNKNOWN:
      return '-';
    case CELL_FLAG:
      return 'f';
    case CELL_MINE:
      return 'm';
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

    return ((hash<int>()(cell.row) ^ (hash<int>()(cell.col) << 1)) >> 1) ^
           (hash<int>()(cell.value) << 1);
  }
};

} // namespace std

#endif // AUTOSWEEP_CELL_H_
