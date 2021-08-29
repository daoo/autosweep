#include <opencv4/opencv2/opencv.hpp>

#include <iostream>

#include "board.h"
#include "stream.h"

void Test1() {
  Board board = Board::FromString("-1\n"
                                  "10\n");
  std::unordered_set<Cell> new_flags, new_clicks;

  Changes(board, new_flags, new_clicks);

  assert(new_flags.size() == 1);
  assert(new_flags.cbegin()->row == 0 && new_flags.cbegin()->col == 0);
  assert(new_clicks.empty());
}

int main() {
  Test1();
  return 0;
}
