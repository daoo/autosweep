#include <opencv4/opencv2/opencv.hpp>

#include <iostream>
#include <unordered_set>
#include <vector>

#include "board.h"
#include "desktop.h"
#include "stream.h"

int main() {
  int x1 = 851;
  int y1 = 114;
  int x2 = 1351;
  int y2 = 432;
  Desktop desktop(x1, y1, x2 - x1, y2 - y1);

  cv::Mat image;
  desktop.Capture(image);
  // cv::imwrite("image.bmp", image);

  /* image = cv::imread("image.bmp"); */

  Board board = Board::FromScreenshot(image);
  std::cout << board;

  std::unordered_set<Cell> new_flags, new_clicks;
  Changes(board, new_flags, new_clicks);
  for (auto iter = new_flags.cbegin(); iter != new_flags.cend(); ++iter) {
    Cell cell = *iter;
    // TODO: Unify X_OFFSET, Y_OFFSET
    // plus 8 for clicking in middle
    std::cout << "RightClick(" << cell.row << ", " << cell.col << ") "
              << cell.value << '\n';
    int x = 8 + x1 + 10 + cell.col * 16;
    int y = 8 + y1 + 52 + cell.row * 16;
    desktop.RightClick(x, y);
  }
  for (auto iter = new_clicks.cbegin(); iter != new_clicks.cend(); ++iter) {
    // TODO: Unify X_OFFSET, Y_OFFSET
    // plus 8 for clicking in middle
    Cell cell = *iter;
    std::cout << "LeftClick(" << cell.row << ", " << cell.col << ") "
              << cell.value << '\n';
    int x = 8 + x1 + 10 + cell.col * 16;
    int y = 8 + y1 + 52 + cell.row * 16;
    desktop.LeftClick(x, y);
  }

  /* cv::imshow("image", image); */
  /* char k; */
  /* do { */
  /*   k = cv::waitKey(100); */
  /* } while (k != 'q'); */
}
