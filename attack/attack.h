//
// Created by Homin Su on 2021/10/25.
//

#ifndef TEST_ATTACK_ATTACK_H_
#define TEST_ATTACK_ATTACK_H_

#include <memory>

#include <opencv4/opencv2/opencv.hpp>

class Patch;

/**
 * @brief Attack the images
 */
class Attack {
 private:
  std::shared_ptr<Patch> patch_;  ///< Patch

 public:
  Attack();
  ~Attack();

  /**
   * @brief This func will write the sawtooth into cv::Mat according to detected box rect
   * @param _boxes detected box rect
   * @param _mat cv::Mat
   * @return cv::Mat
   */
  cv::Mat &Write(std::shared_ptr<std::vector<Box::Rect>> &_boxes, cv::Mat &_mat);
};

#endif //TEST_ATTACK_ATTACK_H_
