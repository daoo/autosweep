#include "autosweep/board.h"
#include "autosweep/boardstream.h"
#include "autosweep/cell.h"

#include <istream>
#include <ostream>

std::istream& operator>>(std::istream& stream, CellValue& cell) {
  std::istream::sentry sentry(stream);
  if (sentry) {
    int next = stream.get();
    if (next != std::istream::traits_type::eof()) {
      auto converted = Cell::FromChar(static_cast<char>(next));
      if (converted) {
        cell = *converted;
      }
    }
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const Board& board) {
  std::ostream::sentry sentry(stream);
  if (sentry) {
    for (size_t i = 0; i < board.rows(); ++i) {
      for (size_t j = 0; j < board.cols(); ++j) {
        stream.rdbuf()->sputc(board.at(i, j).ToChar());
      }
      stream.rdbuf()->sputc('\n');
    }
  }
  return stream;
}
