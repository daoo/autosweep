#include "board.h"

#include <opencv4/opencv2/opencv.hpp>

namespace {
void CountNeighbor(const Board &board, int row, int col, int *unknown_count,
                   int *flag_count) {
  if (row < 0 || row >= board.rows())
    return;
  if (col < 0 || col >= board.cols())
    return;
  Cell cell = board.at(row, col);
  *unknown_count += cell.IsUnknown() ? 1 : 0;
  *flag_count += cell.IsFlag() ? 1 : 0;
}

void CountNeighbors(const Board &board, int row, int col, int *unknown_count,
                    int *flag_count) {
  CountNeighbor(board, row - 1, col - 1, unknown_count, flag_count);
  CountNeighbor(board, row - 1, col + 0, unknown_count, flag_count);
  CountNeighbor(board, row - 1, col + 1, unknown_count, flag_count);
  CountNeighbor(board, row + 0, col + 1, unknown_count, flag_count);
  CountNeighbor(board, row + 1, col + 1, unknown_count, flag_count);
  CountNeighbor(board, row + 1, col + 0, unknown_count, flag_count);
  CountNeighbor(board, row + 1, col - 1, unknown_count, flag_count);
  CountNeighbor(board, row + 0, col - 1, unknown_count, flag_count);
}

void ListUnknownNeighbor(const Board &board, int row, int col,
                         std::unordered_set<Cell> *unknown_neighbors) {
  if (row < 0 || row >= board.rows())
    return;
  if (col < 0 || col >= board.cols())
    return;
  Cell cell = board.at(row, col);
  if (cell.IsUnknown())
    unknown_neighbors->insert(cell);
}

void ListUnknownNeighbors(const Board &board, int row, int col,
                          std::unordered_set<Cell> *unknown_neighbors) {
  ListUnknownNeighbor(board, row - 1, col - 1, unknown_neighbors);
  ListUnknownNeighbor(board, row - 1, col + 0, unknown_neighbors);
  ListUnknownNeighbor(board, row - 1, col + 1, unknown_neighbors);
  ListUnknownNeighbor(board, row + 0, col + 1, unknown_neighbors);
  ListUnknownNeighbor(board, row + 1, col + 1, unknown_neighbors);
  ListUnknownNeighbor(board, row + 1, col + 0, unknown_neighbors);
  ListUnknownNeighbor(board, row + 1, col - 1, unknown_neighbors);
  ListUnknownNeighbor(board, row + 0, col - 1, unknown_neighbors);
}

int CellExpectedMineCount(const cv::Mat &board, int row, int col) {
  if (row < 0 || row >= board.rows)
    return 0;
  if (col < 0 || col >= board.cols)
    return 0;
  uchar cell = board.at<uchar>(row, col);
  return cell >= 1 && cell <= 9 ? cell : 0;
}

int NeighboringExpectedMineCount(const cv::Mat &board, int row, int col) {
  return CellExpectedMineCount(board, row - 1, col - 1) +
         CellExpectedMineCount(board, row - 1, col + 0) +
         CellExpectedMineCount(board, row - 1, col + 1) +
         CellExpectedMineCount(board, row + 0, col + 1) +
         CellExpectedMineCount(board, row + 1, col + 1) +
         CellExpectedMineCount(board, row + 1, col + 0) +
         CellExpectedMineCount(board, row + 1, col - 1) +
         CellExpectedMineCount(board, row + 0, col - 1);
}
} // namespace

Board Board::FromString(const std::string &string) {
  // Find size
  int rows = 0;
  int cols = 0;
  size_t previous_position = 0;
  size_t position = 0;
  while ((position = string.find('\n', position + 1)) != std::string::npos) {
    int current_column = position - previous_position;
    if (cols > 0 && current_column != cols) {
      throw std::runtime_error("FromString() error");
    }
    cols = current_column;
    previous_position = position + 1;
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

// Algorithm:
//
// A cell is satisfied if its number matches the number of neighboring flags.
//
// If the number of unknown neighbors plus the number of neighboring flags is
// equal to the cell's number, the neighbors can be flagged.
// If the cell is satisified, all unknown neighbors can be clicked.
void Changes(const Board &board, std::unordered_set<Cell> &new_flags,
             std::unordered_set<Cell> &new_clicks) {
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      Cell cell = board.at(row, col);
      if (cell.IsNumber()) {
        int unknown_count = 0, flag_count = 0;
        CountNeighbors(board, row, col, &unknown_count, &flag_count);
        if (cell.value == flag_count) {
          ListUnknownNeighbors(board, row, col, &new_clicks);
        }
        if (cell.value == unknown_count + flag_count) {
          ListUnknownNeighbors(board, row, col, &new_flags);
        }
      }
    }
  }
}

Cell ACellWithMostNeighboringMines(const Board &board) {
  cv::Mat recounted(board.rows(), board.cols(), CV_8UC1);
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      Cell cell = board.at(row, col);
      if (cell.IsNumber()) {
        int unknown_count = 0, flag_count = 0;
        CountNeighbors(board, row, col, &unknown_count, &flag_count);
        recounted.at<uchar>(row, col) =
            static_cast<uchar>(cell.value - flag_count);
      } else {
        recounted.at<uchar>(row, col) = cell.value;
      }
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
