#include <catch2/catch.hpp>
#include <iostream>
#include <opencv4/opencv2/core/mat.hpp>

#include "autosweep/network.h"

TEST_CASE("Network::Network", "[Neural]") {
  Network network({2, 3, 1});

  REQUIRE(network.biases().size() == 2);
  REQUIRE(network.biases().at(0).size() == cv::Size2i{1, 3});
  REQUIRE(network.biases().at(1).size() == cv::Size2i{1, 1});

  REQUIRE(network.weights().size() == 2);
  REQUIRE(network.weights().at(0).size() == cv::Size2i{2, 3});
  REQUIRE(network.weights().at(1).size() == cv::Size2i{3, 1});
}

TEST_CASE("Network::FeedForward", "[Neural]") {
  Network network({2, 3, 1});
  cv::Mat input({0.0, 1.0});

  cv::Mat result = network.FeedForward(input);

  REQUIRE(result.size() == cv::Size2i{1, 1});
}

TEST_CASE("Network::BackPropagate", "[Neural]") {
  Network network({2, 3, 1});
  cv::Mat input({0.0, 1.0});
  cv::Mat output({1.0});

  auto result = network.BackPropagate(input, output);

  REQUIRE(result.b.size() == 2);
  REQUIRE(result.b[0].size() == cv::Size2i{1, 3});
  REQUIRE(result.b[1].size() == cv::Size2i{1, 1});
  REQUIRE(result.w.size() == 2);
  REQUIRE(result.w[0].size() == cv::Size2i{2, 3});
  REQUIRE(result.w[1].size() == cv::Size2i{3, 1});
}

TEST_CASE("Network::Update", "[Neural]") {
  Network network({2, 3, 1});
  cv::Mat input({0.0, 1.0});
  cv::Mat output({1.0});
  double eta = 3.0;

  for (int i = 0; i < 20; ++i) {
    network.Update(network.BackPropagate(input, output), eta);
  }

  double result = network.FeedForward(input).at<double>(0, 0);
  REQUIRE(result >= 0.95);
}
