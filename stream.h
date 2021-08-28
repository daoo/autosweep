#ifndef AUTOSWEEP_STREAM_H_
#define AUTOSWEEP_STREAM_H_

#include "cell.h"
#include "board.h"

/* #include <ostream> */

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

#endif // AUTOSWEEP_STREAM_H_
