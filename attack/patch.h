//
// Created by Homin Su on 2021/10/24.
//

#ifndef YOLOV5_ATTACK_ATTACK_PATCH_H_
#define YOLOV5_ATTACK_ATTACK_PATCH_H_

#include "../defs/box.h"

#include "saw_tooth.h"

/**
 * @brief Add a patch into a cv::Mat according to the BoxRect
 */
class Patch : protected SawTooth {
 private:
  int interval_;

 public:
  Patch();
  ~Patch() override;

 public:
  void AddPatch(const Box::Rect &_box, cv::Mat& _mat);
};

#endif //YOLOV5_ATTACK_ATTACK_PATCH_H_
