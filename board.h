#ifndef AUTOSWEEP_BOARD_H_
#define AUTOSWEEP_BOARD_H_

#include <opencv4/opencv2/core/mat.hpp>
#include <unordered_set>
#include <vector>

#include "cell.h"

class Board {
private:
  int rows_ = 16;
  int cols_ = 30;
  cv::Mat cells_;

public:
  Board(cv::Mat cells);

  int rows() const { return rows_; }
  int cols() const { return cols_; }

  Cell at(int row, int col) const {
    return Cell{row, col, cells_.at<uint8_t>(row, col)};
  }

  void PartitionCell(int row, int col, std::vector<Cell> &unknowns,
                     std::vector<Cell> &flags) const;

  void PartitionNeighbors(int row, int col, std::vector<Cell> &unknowns,
                          std::vector<Cell> &flags) const;

  void Changes(std::unordered_set<Cell> &new_flags,
               std::unordered_set<Cell> &new_clicks) const;
};

#endif // AUTOSWEEP_BOARD_H_
