#include "autosweep/board.h"
#include "autosweep/boardstream.h"
#include "autosweep/cell.h"

std::ostream& operator<<(std::ostream& stream, const Cell& cell) {
  stream << cell.ToChar();
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const Board& board) {
  for (size_t i = 0; i < board.rows(); ++i) {
    for (size_t j = 0; j < board.cols(); ++j) {
      stream << board.at(i, j);
    }
    stream << '\n';
  }
  return stream;
}
