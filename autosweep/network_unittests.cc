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
  SECTION("2 layers with 1 neuron") {
    Network network({1, 1});
    cv::Mat input({1.0});

    cv::Mat result = network.FeedForward(input);

    REQUIRE(result.size() == cv::Size2i{1, 1});
  }
  SECTION("3 layers with different neuron counts") {
    Network network({2, 3, 1});
    cv::Mat input({0.0, 1.0});

    cv::Mat result = network.FeedForward(input);

    REQUIRE(result.size() == cv::Size2i{1, 1});
  }
  SECTION("5 layers with different neuron counts") {
    Network network({6, 10, 3, 8, 2});
    cv::Mat input({0.0, 0.2, 0.4, 0.6, 0.8, 1.0});

    cv::Mat result = network.FeedForward(input);

    REQUIRE(result.size() == cv::Size2i{1, 2});
  }
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
  SECTION("2 layers with 1 neuron") {
    Network network({1, 1});
    cv::Mat input({1.0});
    cv::Mat output({1.0});
    double eta = 10.0;

    for (int i = 0; i < 10; ++i) {
      network.Update(network.BackPropagate(input, output), eta);
    }

    cv::Mat result = network.FeedForward(input);
    REQUIRE(result.at<double>(0) >= 0.9);
  }

  SECTION("2 layers with 2 neurons") {
    Network network({2, 2});
    cv::Mat input({0.0, 1.0});
    cv::Mat output({0.0, 1.0});
    double eta = 10.0;

    for (int i = 0; i < 10; ++i) {
      network.Update(network.BackPropagate(input, output), eta);
    }

    cv::Mat result = network.FeedForward(input);
    REQUIRE(result.at<double>(0) <= 0.05);
    REQUIRE(result.at<double>(1) >= 0.95);
  }

  SECTION("3 layers with different neuron count") {
    Network network({2, 3, 1});
    cv::Mat input({0.0, 1.0});
    cv::Mat output({1.0});
    double eta = 10.0;

    for (int i = 0; i < 10; ++i) {
      network.Update(network.BackPropagate(input, output), eta);
    }

    double result = network.FeedForward(input).at<double>(0, 0);
    REQUIRE(result >= 0.95);
  }

  SECTION("3 layers with equal neuron count") {
    Network network({2, 2, 2});
    cv::Mat input({0.0, 1.0});
    cv::Mat output({0.0, 1.0});
    double eta = 10.0;

    for (int i = 0; i < 20; ++i) {
      network.Update(network.BackPropagate(input, output), eta);
    }

    cv::Mat result = network.FeedForward(input);
    REQUIRE(result.at<double>(0) <= 0.05);
    REQUIRE(result.at<double>(1) >= 0.95);
  }
}
