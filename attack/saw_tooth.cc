//
// Created by Homin Su on 2021/10/25.
//

#include "saw_tooth.h"

SawTooth::SawTooth() : Pixel(), sawtooth_span_(Config::Get()->BasicSetting()->sawtooth_span()) {
  struct_height_ = pixel_size_ * sawtooth_span_;
  struct_width_ = pixel_size_ * (sawtooth_span_ * 2 - 1);
}

SawTooth::~SawTooth() = default;

/**
 * @brief Add a sawtooth into cv::Mat
 * @param _src cv::Mat
 * @param _point coord
 * @param _length sawtooth total length
 * @param _left left or right, because the padding way is different from left side or right side
 */
void SawTooth::AddSawTooth(cv::Mat &_src, const cv::Point &_point, int _length, bool _left) {
  int padding_num = _length / struct_width_;
  if (_left) {
    cv::Point current_point;
    current_point = _point;
    for (int i = _point.x, end = _point.x - padding_num * struct_width_; i > end; i -= struct_width_) {
      current_point.x -= pixel_size_;
      // '\'
      for (int x = current_point.x, y = current_point.y, x_end = current_point.x - sawtooth_span_ * pixel_size_;
           x > x_end;
           x -= pixel_size_, y -= pixel_size_) {
        current_point.x = x;
        current_point.y = y;
        AddPixel(current_point, _src);
      }
      // '/'
      for (int x = current_point.x, y = current_point.y, x_end = current_point.x - sawtooth_span_ * pixel_size_;
           x > x_end;
           x -= pixel_size_, y += pixel_size_) {
        current_point.x = x;
        current_point.y = y;
        AddPixel(current_point, _src);
      }
    }
  } else {
    cv::Point current_point;
    current_point = _point;
    for (int i = _point.x, end = _point.x + padding_num * struct_width_; i < end; i += struct_width_) {
      current_point.x += pixel_size_;
      // '\'
      for (int x = current_point.x, y = current_point.y, x_end = current_point.x + sawtooth_span_ * pixel_size_;
           x < x_end;
           x += pixel_size_, y -= pixel_size_) {
        current_point.x = x;
        current_point.y = y;
        AddPixel(current_point, _src);
      }
      // '/'
      for (int x = current_point.x, y = current_point.y, x_end = current_point.x + sawtooth_span_ * pixel_size_;
           x < x_end;
           x += pixel_size_, y += pixel_size_) {
        current_point.x = x;
        current_point.y = y;
        AddPixel(current_point, _src);
      }
    }
  }
}
