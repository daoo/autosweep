#include <catch2/catch.hpp>

#include "autosweep/board.h"
#include "autosweep/neuralboard.h"

TEST_CASE("GenerateInputLayer", "[Neural]") {
  Board board = Board::FromString(
      "-1\n"
      "10\n");

  cv::Mat result = GenerateInputLayer(board);

  REQUIRE(result.size() == cv::Size2i{1, 4});
  REQUIRE(result.at<double>(0, 0) == Approx(0.0));
  REQUIRE(result.at<double>(1, 0) == Approx(1.0));
  REQUIRE(result.at<double>(2, 0) == Approx(1.0));
  REQUIRE(result.at<double>(3, 0) == Approx(0.0));
}

TEST_CASE("GenerateOutputLayer", "[Neural]") {
  Board board = Board::FromString(
      "m1\n"
      "10\n");

  cv::Mat result = GenerateOutputLayer(board);

  REQUIRE(result.size() == cv::Size2i{1, 4});
  REQUIRE(result.at<double>(0, 0) == Approx(1.0));
  REQUIRE(result.at<double>(1, 0) == Approx(0.0));
  REQUIRE(result.at<double>(2, 0) == Approx(0.0));
  REQUIRE(result.at<double>(3, 0) == Approx(0.0));
}
