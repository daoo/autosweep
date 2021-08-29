#include <opencv4/opencv2/opencv.hpp>

#include <iostream>
#include <unordered_set>
#include <vector>

#include "board.h"
#include "desktop.h"
#include "stream.h"

const int X1 = 851;
const int Y1 = 114;
const int X2 = 1351;
const int Y2 = 432;

void LeftClick(const Desktop &desktop, Cell cell) {
  std::cout << "LeftClick(" << cell.row << ", " << cell.col << ") "
            << cell.value << '\n';
  // TODO: Unify X_OFFSET, Y_OFFSET
  // plus 8 for clicking in middle
  int x = 8 + X1 + 10 + cell.col * 16;
  int y = 8 + Y1 + 52 + cell.row * 16;
  desktop.LeftClick(x, y);
}

void RightClick(const Desktop &desktop, Cell cell) {
  // TODO: Unify X_OFFSET, Y_OFFSET
  // plus 8 for clicking in middle
  std::cout << "RightClick(" << cell.row << ", " << cell.col << ") "
            << cell.value << '\n';
  int x = 8 + X1 + 10 + cell.col * 16;
  int y = 8 + Y1 + 52 + cell.row * 16;
  desktop.RightClick(x, y);
}

bool ComputeAndClick(const Desktop &desktop, const Board &board) {
  std::unordered_set<Cell> new_flags, new_clicks;
  Changes(board, new_flags, new_clicks);
  if (new_flags.empty() && new_clicks.empty())
    return false;

  for (auto iter = new_flags.cbegin(); iter != new_flags.cend(); ++iter)
    RightClick(desktop, *iter);
  for (auto iter = new_clicks.cbegin(); iter != new_clicks.cend(); ++iter)
    LeftClick(desktop, *iter);

  return true;
}

int main() {
  Desktop desktop(X1, Y1, X2 - X1, Y2 - Y1);

  /* cv::imwrite("image.bmp", image); */
  /* image = cv::imread("image.bmp"); */

  {
    cv::Mat image;
    desktop.Capture(image);
    Board board = Board::FromScreenshot(image);
    std::cout << board;

    if (board.IsEmpty()) {
      int center_x = X1 + 8 + 10 + board.cols() * 16 / 2;
      int center_y = Y1 + 8 + 52 + board.rows() * 16 / 2;
      std::cout << "LeftClick(" << center_x << ", " << center_y << ")\n";
      desktop.LeftClick(center_x, center_y);
    }
  }

  while (true) {
    cv::Mat image;
    desktop.Capture(image);
    Board board = Board::FromScreenshot(image);
    std::cout << '\n' << board;
    if (!ComputeAndClick(desktop, board)) {
      Cell cell = ACellWithMostNeighboringMines(board);
      std::cout << "Guessing cell " << cell.row << " x " << cell.col << '\n';
      LeftClick(desktop, cell);
    }
  }

  /* cv::imshow("image", image); */
  /* char k; */
  /* do { */
  /*   k = cv::waitKey(100); */
  /* } while (k != 'q'); */
}
