#ifndef AUTOSWEEP_STREAM_H_
#define AUTOSWEEP_STREAM_H_

#include <ostream>

#include "autosweep/cell.h"
#include "autosweep/board.h"

std::ostream &operator<<(std::ostream &stream, const Cell &cell);
std::ostream &operator<<(std::ostream &stream, const Board &board);

#endif // AUTOSWEEP_STREAM_H_
