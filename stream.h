#ifndef AUTOSWEEP_STREAM_H_
#define AUTOSWEEP_STREAM_H_

#include "cell.h"
#include "board.h"

#include <ostream>

std::ostream &operator<<(std::ostream &stream, const Cell &cell);
std::ostream &operator<<(std::ostream &stream, const Board &board);

#endif // AUTOSWEEP_STREAM_H_
