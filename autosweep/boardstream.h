#ifndef AUTOSWEEP_BOARDSTREAM_H_
#define AUTOSWEEP_BOARDSTREAM_H_

#include <iosfwd>

enum CellValue : uint8_t;
class Board;

std::istream& operator>>(std::istream& stream, CellValue& cell);

std::ostream& operator<<(std::ostream& stream, const Board& board);

#endif  // AUTOSWEEP_BOARDSTREAM_H_
