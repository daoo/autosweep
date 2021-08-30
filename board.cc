#include "board.h"

#include <opencv4/opencv2/opencv.hpp>

Board Board::FromString(const std::string &string) {
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
      cells.at<uchar>(i, j) = Cell::FromChar(c);
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

int Aoeu(const cv::Mat &board, int row, int col) {
  if (row < 0 || row >= board.rows)
    return 0;
  if (col < 0 || col >= board.cols)
    return 0;
  uchar cell = board.at<uchar>(row, col);
  return cell >= 1 && cell <= 9 ? cell : 0;
}

int NeighboringExpectedMineCount(const cv::Mat &board, int row, int col) {
  return Aoeu(board, row - 1, col - 1) + Aoeu(board, row - 1, col + 0) +
         Aoeu(board, row - 1, col + 1) + Aoeu(board, row + 0, col + 1) +
         Aoeu(board, row + 1, col + 1) + Aoeu(board, row + 1, col + 0) +
         Aoeu(board, row + 1, col - 1) + Aoeu(board, row + 0, col - 1);
}

Cell ACellWithMostNeighboringMines(const Board &board) {
  cv::Mat recounted(board.rows(), board.cols(), CV_8UC1);
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      Cell cell = board.at(row, col);
      std::vector<Cell> unknown_neighbors;
      std::vector<Cell> flagged_neighbors;
      PartitionNeighbors(board, row, col, unknown_neighbors, flagged_neighbors);
      recounted.at<uchar>(row, col) =
          cell.IsNumber() ? cell.value - flagged_neighbors.size() : cell.value;
    }
  }

  int storedRow = -1;
  int storedCol = -1;
  int storedCount = 0;
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      if (board.at(row, col).IsUnknown()) {
        int count = NeighboringExpectedMineCount(recounted, row, col);
        if (count > storedCount) {
          storedRow = row;
          storedCol = col;
          storedCount = count;
        }
      }
    }
  }
  return Cell{storedRow, storedCol, CELL_UNKNOWN};
}
