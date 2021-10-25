//
// Created by Homin Su on 2021/10/25.
//

#ifndef TEST_ATTACK_ATTACK_H_
#define TEST_ATTACK_ATTACK_H_

#include <memory>

#include <opencv4/opencv2/opencv.hpp>

class Patch;

class Attack {
 private:
  std::shared_ptr<Patch> patch_;

 public:
  Attack();
  ~Attack();

  cv::Mat &Write(std::shared_ptr<std::vector<Box::Rect>> &_boxes, cv::Mat &_mat);
};

#endif //TEST_ATTACK_ATTACK_H_
