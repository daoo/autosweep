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

  struct Layer {
    cv::Mat biases;
    cv::Mat weights;
  };

  const std::vector<Layer>& layers() const { return layers_; }

 private:
  std::vector<Layer> layers_;
};

#endif  // AUTOSWEEP_NETWORK_H_
