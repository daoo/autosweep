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

void Test2() {
  Board board = Board::FromString("--1\n"
                                  "--2\n"
                                  "12f\n");

  Cell cell = ACellWithMostNeighboringMines(board);

  assert(cell.row == 1 && cell.col == 1);
}

void Test3() {
  Board board = Board::FromString("------\n"
                                  "--111-\n"
                                  "-2101-\n"
                                  "--212-\n"
                                  "---1--\n"
                                  "------\n");

  std::unordered_set<Cell> new_flags, new_clicks;
  Changes(board, new_flags, new_clicks);

  assert(new_flags.empty());
  assert(new_clicks.empty());
}

int main() {
  Test1();
  Test2();
  Test3();
  return 0;
}
