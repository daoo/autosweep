#include "autosweep/stream.h"

std::ostream &operator<<(std::ostream &stream, const Cell &cell) {
  stream << cell.ToChar();
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const Board &board) {
  for (int i = 0; i < board.rows(); ++i) {
    for (int j = 0; j < board.cols(); ++j) {
      stream << board.at(i, j);
    }
    stream << '\n';
  }
  return stream;
}
