//
// Created by Homin Su on 2021/10/24.
//

#include "patch.h"

Patch::Patch() : SawTooth(), interval_(Config::Get()->BasicSetting()->interval()) {}

Patch::~Patch() = default;

/**
 * @brief Add the patch into cv::Mat according to the box rect
 * @param _box box rect
 * @param _mat cv::Mat
 */
void Patch::AddPatch(const Box::Rect &_box, cv::Mat &_mat) {
  if (_box.height() < struct_height_ * 5 || _box.width() < struct_width_ * 5) {
    return;
  }

  auto center_line_x = _box.getMPoint().x;
  auto center_line_y_top = _box.y() + struct_width_;
  auto center_line_y_bottom = _box.getPointBottomLeft().y - struct_width_;

  auto sawtooth_x_left_length = center_line_x - _box.x() - struct_width_;
  auto sawtooth_x_right_length = _box.getPointTopRight().x - center_line_x - struct_width_;

  bool left = true;

  for (int y = center_line_y_top; y < center_line_y_bottom; y += interval_) {
    if (left) {
      AddSawTooth(_mat, {center_line_x, y}, sawtooth_x_left_length, left);
      left = !left;
    } else {
      AddSawTooth(_mat, {center_line_x, y}, sawtooth_x_right_length, left);
      left = !left;
    }
  }

  for (int y = center_line_y_top; y < center_line_y_bottom; y += pixel_size_) {
    AddPixel({center_line_x, y}, _mat);
  }

}
