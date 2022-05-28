#ifndef AUTOSWEEP_BOARDSTREAM_H_
#define AUTOSWEEP_BOARDSTREAM_H_

#include <ostream>

struct Cell;
class Board;

std::ostream& operator<<(std::ostream& stream, const Cell& cell);
std::ostream& operator<<(std::ostream& stream, const Board& board);

#endif  // AUTOSWEEP_BOARDSTREAM_H_
