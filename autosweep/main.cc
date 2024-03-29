#include <iostream>
#include <opencv4/opencv2/core.hpp>
#include <random>
#include <unordered_set>

#include "autosweep/board.h"
#include "autosweep/boardstream.h"
#include "autosweep/desktop.h"
#include "autosweep/detector.h"

namespace {

void LeftClick(
    const Desktop& desktop, const BoardLocation& location, Cell cell) {
  std::cout << "LeftClick(" << cell.row << ", " << cell.col << ")\n";
  desktop.LeftClick(location.CellCenter(cell.row, cell.col));
}

void RightClick(
    const Desktop& desktop, const BoardLocation& location, Cell cell) {
  std::cout << "RightClick(" << cell.row << ", " << cell.col << ")\n";
  desktop.RightClick(location.CellCenter(cell.row, cell.col));
}

bool ComputeAndClick(
    const Desktop& desktop, const BoardLocation& location, const Board& board) {
  std::unordered_set<Cell> new_flags, new_clicks;
  ComputeKnownNeighboringCellStates(board, &new_flags, &new_clicks);
  if (new_flags.empty() && new_clicks.empty()) return false;

  for (auto iter = new_flags.cbegin(); iter != new_flags.cend(); ++iter)
    RightClick(desktop, location, *iter);
  for (auto iter = new_clicks.cbegin(); iter != new_clicks.cend(); ++iter)
    LeftClick(desktop, location, *iter);

  return true;
}

}  // namespace

int main() {
  Desktop desktop;
  cv::Rect display_rectangle = desktop.GetDisplayRectangle();
  std::cout << "Taking screenshot of " << display_rectangle << "\n";
  cv::Mat initial_screenshot = desktop.Capture(display_rectangle);
  BoardLocation location = BoardLocation::Find(initial_screenshot);
  {
    Board board = ParseBoard(initial_screenshot(location.Board()));

    if (board.IsEmpty()) {
      std::cout << "Board empty, clicking middle.\n";
      LeftClick(
          desktop, location, board.at(board.rows() / 2, board.cols() / 2));
    }
  }

  std::minstd_rand generator(20220402);
  while (true) {
    std::cout << "Taking screenshot of " << location.Board() << "\n";
    Board board = ParseBoard(desktop.Capture(location.Board()));
    if (board.IsWon()) {
      std::cout << "Victory!\n";
      return 0;
    }
    if (board.IsLost()) {
      std::cout << "Lost!\n" << board;
      return 0;
    }
    if (!ComputeAndClick(desktop, location, board)) {
      std::cout << "No neighboring moves found, guessing.\n";
      std::vector<Cell> unknowns = UnknownCells(board);
      std::uniform_int_distribution<> distribution(0, unknowns.size() - 1);
      LeftClick(desktop, location, unknowns[distribution(generator)]);
    }
  }
}
