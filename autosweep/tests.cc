#include "autosweep/board.h"
#include "autosweep/stream.h"

#include <iostream>

#include <opencv4/opencv2/opencv.hpp>

void FromString_ExampleInput_ExpectedBoard() {
  std::string string("-1\n"
                     "10\n");

  Board board = Board::FromString(string);

  assert(board.rows() == 2);
  assert(board.cols() == 2);
  assert(board.at(0, 0).value == CELL_UNKNOWN);
  assert(board.at(0, 1).value == 1);
  assert(board.at(1, 0).value == 1);
  assert(board.at(1, 1).value == 0);
}

void Test1() {
  Board board = Board::FromString("-1\n"
                                  "10\n");

  std::unordered_set<Cell> new_flags, new_clicks;
  Changes(board, &new_flags, &new_clicks);

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
  Changes(board, &new_flags, &new_clicks);

  assert(new_flags.empty());
  assert(new_clicks.empty());
}

int main() {
  FromString_ExampleInput_ExpectedBoard();
  Test1();
  Test2();
  Test3();
  return 0;
}
