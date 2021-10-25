//
// Created by Homin Su on 2021/10/25.
//

#ifndef YOLOV5_ATTACK_ATTACK_SAW_TOOTH_H_
#define YOLOV5_ATTACK_ATTACK_SAW_TOOTH_H_

#include "pixel.h"

/**
 * @brief Add a sawtooth into a cv::Mat
 */
class SawTooth : protected Pixel {
 protected:
  int sawtooth_span_; ///< the span of the sawtooth
  int struct_height_; ///< height of the each sawtooth struct('/|')
  int struct_width_;  ///< width of the each sawtooth struct('/|')

 public:
  SawTooth();
  ~SawTooth() override;

 protected:
  /**
   * @brief Add a sawtooth into cv::Mat
   * @param _src cv::Mat
   * @param _point coord
   * @param _length sawtooth total length
   * @param _left left or right, because the padding way is different from left side or right side
   */
  void AddSawTooth(cv::Mat &_src, const cv::Point &_point, int _length, bool _left);
};

#endif //YOLOV5_ATTACK_ATTACK_SAW_TOOTH_H_
