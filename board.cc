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
  cv::Scalar six_color(162.445312, 162.445312, 127.851562);

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
  if (cv::norm(color, six_color) < 0.1)
    return 6;

  printf("unknown color: %f, %f, %f\n", color[0], color[1], color[2]);
  exit(1);
  return 0;
}

Board::Board(cv::Mat cells) : cells_(cells) {}

Board Board::FromScreenshot(cv::Mat screenshot) {
  const int ROWS = 16;
  const int COLS = 30;
  const int X_OFFSET = 10;
  const int Y_OFFSET = 52;
  const int CELL_WIDTH = 16;
  const int CELL_HEIGHT = 16;
  cv::Mat cells(ROWS, COLS, CV_8UC1);
  for (int i = 0; i < COLS; ++i) {
    int x1 = X_OFFSET + i * CELL_WIDTH;
    int x2 = x1 + CELL_WIDTH;
    for (int j = 0; j < ROWS; ++j) {
      int y1 = Y_OFFSET + j * CELL_HEIGHT;
      int y2 = y1 + CELL_HEIGHT;
      cv::Mat cell = screenshot(cv::Range(y1, y2), cv::Range(x1, x2));
      cells.at<uint8_t>(j, i) = ParseCell(cell);
    }
  }
  return Board(cells);
}

Board Board::FromString(std::string string) {
  // Find size
  int rows = 0;
  int cols = 0;
  size_t prevpos = 0;
  size_t pos = 0;
  while ((pos = string.find('\n', pos + 1)) != std::string::npos) {
    int thiscol = pos - prevpos;
    if (cols > 0 && thiscol != cols) {
      throw std::runtime_error("FromString() error");
    }
    cols = thiscol;
    prevpos = pos + 1;
    ++rows;
  }

  cv::Mat cells(rows, cols, CV_8UC1);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      char c = string.at(i * (cols + 1) + j);
      cells.at<uint8_t>(i, j) = Cell::FromChar(c);
    }
  }
  return Board(cells);
}

void PartitionCell(const Board &board, int row, int col,
                   std::vector<Cell> &unknowns, std::vector<Cell> &flags) {
  if (row < 0 || row >= board.rows())
    return;
  if (col < 0 || col >= board.cols())
    return;
  Cell cell = board.at(row, col);
  if (cell.IsUnknown())
    unknowns.push_back(cell);
  else if (cell.IsFlag())
    flags.push_back(cell);
}

void PartitionNeighbors(const Board &board, int row, int col,
                        std::vector<Cell> &unknowns, std::vector<Cell> &flags) {
  PartitionCell(board, row - 1, col - 1, unknowns, flags);
  PartitionCell(board, row - 1, col + 0, unknowns, flags);
  PartitionCell(board, row - 1, col + 1, unknowns, flags);
  PartitionCell(board, row + 0, col + 1, unknowns, flags);
  PartitionCell(board, row + 1, col + 1, unknowns, flags);
  PartitionCell(board, row + 1, col + 0, unknowns, flags);
  PartitionCell(board, row + 1, col - 1, unknowns, flags);
  PartitionCell(board, row + 0, col - 1, unknowns, flags);
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
void Changes(const Board &board, std::unordered_set<Cell> &new_flags,
             std::unordered_set<Cell> &new_clicks) {
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      Cell cell = board.at(row, col);
      std::vector<Cell> unknown_neighbors;
      std::vector<Cell> flagged_neighbors;
      PartitionNeighbors(board, row, col, unknown_neighbors, flagged_neighbors);
      if (cell.IsNumber()) {
        if (cell.value == flagged_neighbors.size()) {
          std::copy(unknown_neighbors.cbegin(), unknown_neighbors.cend(),
                    std::inserter(new_clicks, new_clicks.end()));
        }
        if (cell.value == flagged_neighbors.size() + unknown_neighbors.size()) {
          std::copy(unknown_neighbors.cbegin(), unknown_neighbors.cend(),
                    std::inserter(new_flags, new_flags.end()));
        }
      }
    }
  }
}
