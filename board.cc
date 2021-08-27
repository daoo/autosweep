#include "board.h"

#include <opencv4/opencv2/opencv.hpp>

uint8_t ParseCell(const cv::Mat &cell) {
  cv::Scalar color = cv::mean(cell);

  cv::Scalar unknown_color(189, 189, 189);
  cv::Scalar flag_color(160.207031, 160.207031, 177.140625);
  cv::Scalar zero_color(181.007812, 181.007812, 181.007812);
  cv::Scalar one_color(191.320312, 151.476562, 151.476562);
  cv::Scalar two_color(133.019531, 164.250000, 133.019531);
  cv::Scalar three_color(135.234375, 135.234375, 196.992188);
  cv::Scalar four_color(166.570312, 139.664062, 139.664062);
  cv::Scalar five_color(129.328125, 129.328125, 162.960938);

  if (cv::norm(color, unknown_color) < 0.1)
    return CELL_UNKNOWN;
  if (cv::norm(color, flag_color) < 0.1)
    return CELL_FLAG;
  if (cv::norm(color, zero_color) < 0.1)
    return 0;
  if (cv::norm(color, one_color) < 0.1)
    return 1;
  if (cv::norm(color, two_color) < 0.1)
    return 2;
  if (cv::norm(color, three_color) < 0.1)
    return 3;
  if (cv::norm(color, four_color) < 0.1)
    return 4;
  if (cv::norm(color, five_color) < 0.1)
    return 5;

  printf("unknown color: %f, %f, %f\n", color[0], color[1], color[2]);
  exit(1);
  return 0;
}

Board::Board(cv::Mat screenshot) : _cells(_rows, _cols, CV_8UC1) {
  const int X_OFFSET = 10;
  const int Y_OFFSET = 52;
  const int CELL_WIDTH = 16;
  const int CELL_HEIGHT = 16;
  for (int i = 0; i < _cols; ++i) {
    int x1 = X_OFFSET + i * CELL_WIDTH;
    int x2 = x1 + CELL_WIDTH;
    for (int j = 0; j < _rows; ++j) {
      int y1 = Y_OFFSET + j * CELL_HEIGHT;
      int y2 = y1 + CELL_HEIGHT;
      cv::Mat cell = screenshot(cv::Range(y1, y2), cv::Range(x1, x2));
      _cells.at<uint8_t>(j, i) = ParseCell(cell);
    }
  }
}

void Board::Printf() const {
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      printf("%c", at(i, j).ToChar());
    }
    printf("\n");
  }
}

void Board::PartitionCell(int row, int col, std::vector<Cell> &unknowns,
                          std::vector<Cell> &flags) const {
  if (row < 0 || row >= _rows)
    return;
  if (col < 0 || col >= _cols)
    return;
  Cell cell = at(row, col);
  if (cell.IsUnknown())
    unknowns.push_back(cell);
  else if (cell.IsFlag())
    flags.push_back(cell);
}

void Board::PartitionNeighbors(int row, int col, std::vector<Cell> &unknowns,
                               std::vector<Cell> &flags) const {
  PartitionCell(row - 1, col - 1, unknowns, flags);
  PartitionCell(row - 1, col + 0, unknowns, flags);
  PartitionCell(row - 1, col + 1, unknowns, flags);
  PartitionCell(row + 0, col + 1, unknowns, flags);
  PartitionCell(row + 1, col + 1, unknowns, flags);
  PartitionCell(row + 1, col + 0, unknowns, flags);
  PartitionCell(row + 1, col - 1, unknowns, flags);
  PartitionCell(row + 0, col - 1, unknowns, flags);
}

// Algorithm:
//
// A cell is satisfied if its number matches the number of neighboring flags.
//
// For a given unknown cell:
//
// It should have a flag if it is the only cell that can satisfiy a neighbor.
// It can be clicked if at least one neighbor is satisified.
//
// Also, for a given known cell:
//
// If the number of unknown neighbors plus the number of neighboring flags is
// equal to the cell's number, the neighbors can be flagged.
// If the cell is satisified, all unknown neighbors can be clicked.
void Board::Changes(std::unordered_set<Cell> &new_flags,
                    std::unordered_set<Cell> &new_clicks) const {
  for (int row = 0; row < _rows; ++row) {
    for (int col = 0; col < _cols; ++col) {
      Cell cell = at(row, col);
      std::vector<Cell> unknown_neighbors;
      std::vector<Cell> flagged_neighbors;
      PartitionNeighbors(row, col, unknown_neighbors, flagged_neighbors);
      if (cell.IsKnown()) {
        if (cell.Value == flagged_neighbors.size()) {
          std::copy(unknown_neighbors.cbegin(), unknown_neighbors.cend(),
                    std::inserter(new_clicks, new_clicks.end()));
        }
        if (cell.Value == flagged_neighbors.size() + unknown_neighbors.size()) {
          std::copy(unknown_neighbors.cbegin(), unknown_neighbors.cend(),
                    std::inserter(new_flags, new_flags.end()));
        }
      }
    }
  }
}
