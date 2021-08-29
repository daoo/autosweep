#ifndef AUTOSWEEP_BOARD_H_
#define AUTOSWEEP_BOARD_H_

#include <opencv4/opencv2/core/mat.hpp>
#include <unordered_set>
#include <vector>

#include "cell.h"

class Board {
private:
  cv::Mat cells_;
  Board(const cv::Mat& cells);

public:
  static Board FromScreenshot(const cv::Mat& screenshot);
  static Board FromString(const std::string& string);

  int rows() const { return cells_.rows; }
  int cols() const { return cells_.cols; }

  bool IsEmpty() const {
    for (int i = 0; i < rows(); ++i) {
      for (int j = 0; j < cols(); ++j) {
        if (!at(i, j).IsUnknown())
          return false;
      }
    }
    return true;
  }

  Cell at(int row, int col) const {
    return Cell{row, col, cells_.at<uint8_t>(row, col)};
  }
};

void Changes(const Board &board, std::unordered_set<Cell> &new_flags,
             std::unordered_set<Cell> &new_clicks);

Cell ACellWithMostNeighboringMines(const Board &board);
#endif // AUTOSWEEP_BOARD_H_
