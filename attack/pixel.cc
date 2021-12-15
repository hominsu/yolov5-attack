//
// Created by hominsu on 2021/10/24.
//

#include "pixel.h"

Pixel::Pixel() : pixel_size_(Config::Get()->BasicSetting()->pixel_size()) {}

Pixel::~Pixel() = default;

/**
 * @brief Add pixel kernel into the cv::Mat
 * @param _point coord
 * @param _src cv::Mat
 * @return cv::Mat
 */
cv::Mat &Pixel::AddPixel(const cv::Point &_point, cv::Mat &_src) {
  for (int row = _point.y, row_end = _point.y + pixel_size_; row < row_end; ++row) {
    for (int col = _point.x, col_end = _point.x + pixel_size_; col < col_end; ++col) {
      auto p = _src.ptr<cv::Vec3b>(row, col);
      if (Config::Get()->BasicSetting()->attack_type() == "thresh_binary_inv") {
        ThreshBinaryInv(p, 127);
      } else if (config::Get()->BasicSetting()->attack_type() == "d_value") {
        DValue(p);
      } else if (config::Get()->BasicSetting()->attack_type() == "set_rgb") {
        SetBGR(p, 255, 255, 255);
      } else {
        std::runtime_error("attack type error");
      }
    }
  }

  return _src;
}
