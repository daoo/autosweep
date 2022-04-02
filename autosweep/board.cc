#include <sstream>

#include "autosweep/board.h"

namespace {
void CountNeighbor(
    const Board& board, int row, int col, int* unknown_count, int* flag_count) {
  if (row < 0 || row >= board.rows()) return;
  if (col < 0 || col >= board.cols()) return;
  Cell cell = board.at(row, col);
  *unknown_count += cell.IsUnknown() ? 1 : 0;
  *flag_count += cell.IsFlag() ? 1 : 0;
}

void CountNeighbors(
    const Board& board, int row, int col, int* unknown_count, int* flag_count) {
  CountNeighbor(board, row - 1, col - 1, unknown_count, flag_count);
  CountNeighbor(board, row - 1, col + 0, unknown_count, flag_count);
  CountNeighbor(board, row - 1, col + 1, unknown_count, flag_count);
  CountNeighbor(board, row + 0, col + 1, unknown_count, flag_count);
  CountNeighbor(board, row + 1, col + 1, unknown_count, flag_count);
  CountNeighbor(board, row + 1, col + 0, unknown_count, flag_count);
  CountNeighbor(board, row + 1, col - 1, unknown_count, flag_count);
  CountNeighbor(board, row + 0, col - 1, unknown_count, flag_count);
}

void ListUnknownNeighbor(
    const Board& board, int row, int col,
    std::unordered_set<Cell>* unknown_neighbors) {
  if (row < 0 || row >= board.rows()) return;
  if (col < 0 || col >= board.cols()) return;
  Cell cell = board.at(row, col);
  if (cell.IsUnknown()) unknown_neighbors->insert(cell);
}

void ListUnknownNeighbors(
    const Board& board, int row, int col,
    std::unordered_set<Cell>* unknown_neighbors) {
  ListUnknownNeighbor(board, row - 1, col - 1, unknown_neighbors);
  ListUnknownNeighbor(board, row - 1, col + 0, unknown_neighbors);
  ListUnknownNeighbor(board, row - 1, col + 1, unknown_neighbors);
  ListUnknownNeighbor(board, row + 0, col + 1, unknown_neighbors);
  ListUnknownNeighbor(board, row + 1, col + 1, unknown_neighbors);
  ListUnknownNeighbor(board, row + 1, col + 0, unknown_neighbors);
  ListUnknownNeighbor(board, row + 1, col - 1, unknown_neighbors);
  ListUnknownNeighbor(board, row + 0, col - 1, unknown_neighbors);
}
}  // namespace

Board::Board(const cv::Mat& cells) : cells_(cells) {
  bool has_known = false;
  bool has_unknown = false;
  bool has_mine = false;
  for (int i = 0; i < rows(); ++i) {
    for (int j = 0; j < cols(); ++j) {
      Cell cell = at(i, j);
      has_known = has_known || cell.IsKnown();
      has_unknown = has_unknown || cell.IsUnknown();
      has_mine = has_mine || cell.IsMine();
    }
  }
  if (!has_known) {
    state_ = BOARD_EMPTY;
  } else if (!has_unknown) {
    state_ = BOARD_WON;
  } else if (has_mine) {
    state_ = BOARD_LOST;
  } else {
    state_ = BOARD_INCOMPLETE;
  }
}

Board Board::FromString(const std::string& string) {
  // Find size
  int rows = 0;
  int cols = 0;
  size_t previous_position = 0;
  size_t position = 0;
  while ((position = string.find('\n', position + 1)) != std::string::npos) {
    int line_length = position - previous_position;
    if (cols > 0 && line_length != cols) {
      std::stringstream error;
      error << "Line ending at position " << position << " has length "
            << line_length << " when expecting length " << cols << ".";
      throw std::runtime_error(error.str());
    }
    cols = line_length;
    previous_position = position + 1;
    ++rows;
  }

  cv::Mat cells(rows, cols, CV_8UC1);
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      char character = string.at(row * (cols + 1) + col);
      auto parsed = Cell::FromChar(character);
      if (!parsed) {
        std::stringstream error;
        error << "Character '" << character
              << "' does not represent a valid cell.";
        throw std::runtime_error(error.str());
      }
      cells.at<uchar>(row, col) = parsed.value();
    }
  }
  return Board(cells);
}

std::vector<Cell> UnknownCells(const Board& board) {
  std::vector<Cell> unknowns;
  unknowns.reserve(board.rows() * board.cols());
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      Cell cell = board.at(row, col);
      if (cell.IsUnknown()) {
        unknowns.emplace_back(cell);
      }
    }
  }
  return unknowns;
}

// Algorithm:
//
// A cell is satisfied if its number matches the number of neighboring flags.
//
// If the number of unknown neighbors plus the number of neighboring flags is
// equal to the cell's number, the neighbors can be flagged.
// If the cell is satisified, all unknown neighbors can be clicked.
void ComputeKnownNeighboringCellStates(
    const Board& board, std::unordered_set<Cell>* new_flags,
    std::unordered_set<Cell>* new_clicks) {
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      Cell cell = board.at(row, col);
      if (cell.IsNumber()) {
        int unknown_count = 0, flag_count = 0;
        CountNeighbors(board, row, col, &unknown_count, &flag_count);
        if (cell.value == flag_count) {
          ListUnknownNeighbors(board, row, col, new_clicks);
        }
        if (cell.value == unknown_count + flag_count) {
          ListUnknownNeighbors(board, row, col, new_flags);
        }
      }
    }
  }
}
