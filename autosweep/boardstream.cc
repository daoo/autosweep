#include "autosweep/board.h"
#include "autosweep/boardstream.h"
#include "autosweep/cell.h"

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
