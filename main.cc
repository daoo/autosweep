#include <opencv4/opencv2/opencv.hpp>

#include <iostream>
#include <unordered_set>

#include "board.h"
#include "desktop.h"
#include "detector.h"
#include "stream.h"

namespace {

void LeftClick(const Desktop &desktop, cv::Point2i top_left, Cell cell) {
  std::cout << "LeftClick(" << cell.row << ", " << cell.col << ")\n";
  cv::Point2i point = CellLocation(top_left, cell.row, cell.col);
  desktop.LeftClick(point.x, point.y);
}

void RightClick(const Desktop &desktop, cv::Point2i top_left, Cell cell) {
  std::cout << "RightClick(" << cell.row << ", " << cell.col << ")\n";
  cv::Point2i point = CellLocation(top_left, cell.row, cell.col);
  desktop.RightClick(point.x, point.y);
}

bool ComputeAndClick(const Desktop &desktop, cv::Point2i top_left,
                     const Board &board) {
  std::unordered_set<Cell> new_flags, new_clicks;
  Changes(board, new_flags, new_clicks);
  if (new_flags.empty() && new_clicks.empty())
    return false;

  for (auto iter = new_flags.cbegin(); iter != new_flags.cend(); ++iter)
    RightClick(desktop, top_left, *iter);
  for (auto iter = new_clicks.cbegin(); iter != new_clicks.cend(); ++iter)
    LeftClick(desktop, top_left, *iter);

  return true;
}

} // namespace

int main() {
  Desktop desktop;
  cv::Rect display_rectangle = desktop.GetDisplayRectangle();
  std::cout << "Taking screenshot of " << display_rectangle << "\n";
  cv::Mat initial_screenshot = desktop.Capture(display_rectangle);
  cv::Rect board_rectangle = FindBoardLocation(initial_screenshot);
  {
    Board board = ParseBoard(initial_screenshot(board_rectangle));

    if (board.IsEmpty()) {
      std::cout << "Board empty, clicking middle.\n";
      LeftClick(desktop, board_rectangle.tl(),
                board.at(board.rows() / 2, board.cols() / 2));
    }
  }

  while (true) {
    std::cout << "Taking screenshot of " << board_rectangle << "\n";
    cv::Mat screenshot = desktop.Capture(board_rectangle);
    Board board = ParseBoard(screenshot);
    if (board.IsWon()) {
      std::cout << "Victory!\n";
      return 0;
    }
    if (!ComputeAndClick(desktop, board_rectangle.tl(), board)) {
      Cell cell = ACellWithMostNeighboringMines(board);
      std::cout << "No 100% moves found, guessing.\n";
      RightClick(desktop, board_rectangle.tl(), cell);
    }
  }
}
