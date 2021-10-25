//
// Created by Homin Su on 2021/10/25.
//

#include "patch.h"

#include "attack.h"

Attack::Attack() {
  patch_ = std::make_shared<Patch>();
}

Attack::~Attack() = default;

/**
 * @brief This func will write the sawtooth into cv::Mat according to detected box rect
 * @param _boxes detected box rect
 * @param _mat cv::Mat
 * @return cv::Mat
 */
cv::Mat &Attack::Write(std::shared_ptr<std::vector<Box::Rect>> &_boxes, cv::Mat &_mat) {
  for (auto &_box: *_boxes) {
    patch_->AddPatch(_box, _mat);
  }

  return _mat;
}
