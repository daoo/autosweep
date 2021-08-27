#ifndef AUTOSWEEP_BOARD_H_
#define AUTOSWEEP_BOARD_H_

#include <opencv4/opencv2/opencv.hpp>
#include <unordered_set>
#include <vector>

#include "cell.h"

class Board {
private:
  int _rows = 16;
  int _cols = 30;
  cv::Mat _cells;

public:
  Board(cv::Mat cells);

  Cell at(int row, int col) const {
    return Cell{row, col, _cells.at<uint8_t>(row, col)};
  }

  void Printf() const;

  void PartitionCell(int row, int col, std::vector<Cell> &unknowns,
                     std::vector<Cell> &flags) const;

  void PartitionNeighbors(int row, int col, std::vector<Cell> &unknowns,
                          std::vector<Cell> &flags) const;

  void Changes(std::unordered_set<Cell> &new_flags,
               std::unordered_set<Cell> &new_clicks) const;
};
#endif // AUTOSWEEP_BOARD_H_
