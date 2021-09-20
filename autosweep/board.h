#ifndef AUTOSWEEP_BOARD_H_
#define AUTOSWEEP_BOARD_H_

#include <string>
#include <unordered_set>

#include <opencv4/opencv2/core/mat.hpp>

#include "autosweep/cell.h"

enum BoardState { BOARD_EMPTY, BOARD_INCOMPLETE, BOARD_WON, BOARD_LOST };

class Board {
private:
  cv::Mat cells_;
  BoardState state_;

public:
  explicit Board(const cv::Mat &cells);
  static Board FromString(const std::string &string);

  int rows() const { return cells_.rows; }
  int cols() const { return cells_.cols; }

  bool IsEmpty() const { return state_ == BOARD_EMPTY; }
  bool IsWon() const { return state_ == BOARD_WON; }
  bool IsLost() const { return state_ == BOARD_LOST; }

  Cell at(int row, int col) const {
    return Cell{row, col, cells_.at<uint8_t>(row, col)};
  }
};

void Changes(const Board &board, std::unordered_set<Cell> *new_flags,
             std::unordered_set<Cell> *new_clicks);

Cell ACellWithMostNeighboringMines(const Board &board);

#endif // AUTOSWEEP_BOARD_H_
