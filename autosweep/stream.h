#ifndef AUTOSWEEP_STREAM_H_
#define AUTOSWEEP_STREAM_H_

#include <ostream>

#include "autosweep/board.h"
#include "autosweep/cell.h"

std::ostream& operator<<(std::ostream& stream, const Cell& cell);
std::ostream& operator<<(std::ostream& stream, const Board& board);

#endif  // AUTOSWEEP_STREAM_H_
