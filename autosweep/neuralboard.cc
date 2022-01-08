#include "autosweep/board.h"
#include "autosweep/neuralboard.h"

cv::Mat GenerateInputLayer(const Board& board) {
  cv::Mat mat(board.rows() * board.cols(), 1, CV_64F);
  double* ptr = mat.ptr<double>();
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      auto value = board.at(row, col).value;
      *ptr = value >= 0 && value <= 9 ? value : 0;
      ++ptr;
    }
  }
  return mat;
}

cv::Mat GenerateOutputLayer(const Board& board) {
  cv::Mat mat(board.rows() * board.cols(), 1, CV_64F);
  double* ptr = mat.ptr<double>();
  for (int row = 0; row < board.rows(); ++row) {
    for (int col = 0; col < board.cols(); ++col) {
      bool is_mine = board.at(row, col).IsMine();
      *ptr = is_mine ? 1 : 0;
      ++ptr;
    }
  }
  return mat;
}
