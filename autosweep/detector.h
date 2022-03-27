#ifndef AUTOSWEEP_DETECTOR_H_
#define AUTOSWEEP_DETECTOR_H_

#include <opencv4/opencv2/core/mat.hpp>

class BoardLocation {
 public:
  static BoardLocation Find(const cv::Mat& image);

  cv::Point2i CellCenter(size_t row, size_t col) const;
  cv::Point2i SmielyCenter() const;

  const cv::Rect& Board() const { return _board; }
  const cv::Rect& Smiely() const { return _smiely; }

 private:
  BoardLocation(cv::Rect board, cv::Rect smiely);

  cv::Rect _board;
  cv::Rect _smiely;
};

class Board;
Board ParseBoard(const cv::Mat& image);

#endif  // AUTOSWEEP_DETECTOR_H_
