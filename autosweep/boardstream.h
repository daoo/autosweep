#ifndef AUTOSWEEP_BOARDSTREAM_H_
#define AUTOSWEEP_BOARDSTREAM_H_

#include <ostream>

class Board;

std::ostream& operator<<(std::ostream& stream, const Board& board);

#endif  // AUTOSWEEP_BOARDSTREAM_H_
