#include <opencv4/opencv2/core.hpp>
#include <random>

#include "autosweep/network.h"

namespace {
template <class Distribution, class Generator>
cv::Mat RandomMat(
    Distribution& distribution, Generator& generator, int rows, int cols) {
  cv::Mat mat(rows, cols, CV_64F);
  for (size_t i = 0; i < mat.total(); ++i) {
    mat.at<double>(i) = distribution(generator);
  }
  return mat;
}

cv::Mat Sigmoid(const cv::Mat& z) {
  cv::Mat temp;
  cv::exp(-z, temp);
  return 1.0 / (1.0 + temp);
}

cv::Mat SigmoidPrime(const cv::Mat& z) {
  cv::Mat temp = Sigmoid(z);
  return temp.mul(1 - temp);
}

cv::Mat CostDerivative(const cv::Mat& activation, const cv::Mat& y) {
  return activation - y;
}
}  // namespace

Network::Network(const std::vector<size_t>& sizes) {
  std::minstd_rand generator(20220116);
  std::uniform_real_distribution<> distribution(0.0, 1.0);

  for (size_t i = 1; i < sizes.size(); ++i) {
    biases_.emplace_back(RandomMat(distribution, generator, sizes[i], 1));
    weights_.emplace_back(
        RandomMat(distribution, generator, sizes[i], sizes[i - 1]));
  }
}

cv::Mat Network::FeedForward(cv::Mat mat) const {
  for (size_t i = 0; i < biases_.size(); ++i) {
    mat = Sigmoid(weights_[i] * mat + biases_[i]);
  }
  return mat;
}

namespace {
template <typename T>
T& at(std::vector<T>& vector, int index) {
  return vector[index <= 0 ? vector.size() + index : index];
}

template <typename T>
const T& at(const std::vector<T>& vector, int index) {
  return vector[index <= 0 ? vector.size() + index : index];
}
}  // namespace

Network::Nabla Network::BackPropagate(
    cv::Mat activation, cv::Mat output_activation) const {
  std::vector<cv::Mat> activations, zs;
  activations.push_back(activation);
  for (size_t i = 0; i < biases_.size(); ++i) {
    auto z = weights_[i] * activation + biases_[i];
    zs.push_back(z);
    activation = Sigmoid(z);
    activations.push_back(activation);
  }

  std::vector<cv::Mat> nabla_b, nabla_w;
  for (size_t i = 0; i < weights_.size(); ++i) {
    nabla_b.emplace_back(biases_[i].size(), CV_64F);
    nabla_w.emplace_back(weights_[i].size(), CV_64F);
  }
  cv::Mat delta = CostDerivative(at(activations, -1), output_activation)
                      .mul(SigmoidPrime(at(zs, -1)));
  at(nabla_b, -1) = delta;
  at(nabla_w, -1) = delta * at(activations, -2).t();
  for (size_t i = 2; i < biases_.size(); ++i) {
    delta = weights_[i + 1].t() * delta * SigmoidPrime(zs[i]);
    at(nabla_b, -i) = delta;
    at(nabla_w, -i) = delta * at(activations, -i - 1).t();
  }
  return {nabla_b, nabla_w};
}

void Network::Update(const Network::Nabla& nabla, double eta) {
  for (size_t i = 0; i < biases_.size(); ++i) {
    biases_[i] = biases_[i] - eta * nabla.b[i];
    weights_[i] = weights_[i] - eta * nabla.w[i];
  }
}
