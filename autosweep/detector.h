#ifndef AUTOSWEEP_DETECTOR_H_
#define AUTOSWEEP_DETECTOR_H_

#include <opencv4/opencv2/core/mat.hpp>

#include "autosweep/board.h"

cv::Rect FindBoardLocation(const cv::Mat& image);
Board ParseBoard(const cv::Mat& image);
cv::Point2i CellLocation(cv::Point2i board_location, int row, int col);

#endif  // AUTOSWEEP_DETECTOR_H_
