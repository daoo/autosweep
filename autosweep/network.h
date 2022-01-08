#ifndef AUTOSWEEP_NETWORK_H_
#define AUTOSWEEP_NETWORK_H_

#include <opencv4/opencv2/core/mat.hpp>
#include <vector>

class Network {
 public:
  explicit Network(const std::vector<size_t>& sizes);

  cv::Mat FeedForward(cv::Mat mat) const;

  struct Nabla {
    std::vector<cv::Mat> b;
    std::vector<cv::Mat> w;
  };
  Nabla BackPropagate(cv::Mat input, cv::Mat output) const;
  void Update(const Nabla& nabla, double eta);

  size_t layers() const { return _biases.size() + 1; }

  const std::vector<cv::Mat>& biases() const { return _biases; }
  const std::vector<cv::Mat>& weights() const { return _weights; }

 private:
  std::vector<cv::Mat> _biases;
  std::vector<cv::Mat> _weights;
};

#endif  // AUTOSWEEP_NETWORK_H_
