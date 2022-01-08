#ifndef AUTOSWEEP_NEURALBOARD_H_
#define AUTOSWEEP_NEURALBOARD_H_

#include <opencv4/opencv2/core/mat.hpp>

class Board;

cv::Mat GenerateInputLayer(const Board& board);
cv::Mat GenerateOutputLayer(const Board& board);

#endif  // AUTOSWEEP_NEURALBOARD_H_
