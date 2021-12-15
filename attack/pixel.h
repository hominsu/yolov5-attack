//
// Created by hominsu on 2021/10/24.
//

#ifndef YOLOV5_ATTACK_ATTACK_PIXEL_H_
#define YOLOV5_ATTACK_ATTACK_PIXEL_H_

#include <opencv4/opencv2/opencv.hpp>

#include "../config/config.h"

/**
 * @brief Add a pixel kernel into a cv::Mat
 * @details Three ways to change the kernel pixel bgr value: 1. SetBGR(): set the bgr value directly. 2. ThreshBinaryInv(): set cv::Mat pointer BGR value base on the inverse threshold. 3. DValue(): set cv::Mat pointer bgr value base on the difference value on the color channel.
 */
class Pixel {
 protected:
  int pixel_size_;  ///< pixel kernel size

 public:
  explicit Pixel();

  virtual ~Pixel();

 protected:
  /**
   * @brief Add pixel kernel into the cv::Mat
   * @param _point coord
   * @param _src cv::Mat
   * @return cv::Mat
   */
  cv::Mat &AddPixel(const cv::Point &_point, cv::Mat &_src);

 private:
  /**
   * @brief Set cv::Mat pointer BGR value
   * @tparam Tp Mat pointer: uchar or ptr template
   * @tparam Tv BGR Value
   * @param _tp pointer
   * @param _b b channel
   * @param _g g channel
   * @param _r r channel
   */
  template<typename Tp, typename Tv>
  inline void SetBGR(Tp &&_tp, Tv &&_b, Tv &&_g, Tv &&_r) {
    _tp->val[0] = _b;
    _tp->val[1] = _g;
    _tp->val[2] = _r;
  };

  /**
   * @brief Set cv::Mat pointer BGR value base on the inverse threshold
   * @tparam Tp Mat pointer: uchar or ptr template
   * @tparam Tc Threshold value
   * @param _tp pointer
   * @param _threshold_value threshold
   */
  template<typename Tp, typename Tc>
  inline void ThreshBinaryInv(Tp &&_tp, Tc &&_threshold_value) {
    _tp->val[0] = _tp->val[0] > _threshold_value ? 0 : 255;
    _tp->val[1] = _tp->val[1] > _threshold_value ? 0 : 255;
    _tp->val[2] = _tp->val[2] > _threshold_value ? 0 : 255;
  };

  /**
   * @brief Set cv::Mat pointer BGR value base on the difference value
   * @tparam Tp Mat pointer: uchar or ptr template
   * @param _tp pointer
   */
  template<typename Tp>
  inline void DValue(Tp &&_tp) {
    _tp->val[0] = 255 - _tp->val[0];
    _tp->val[1] = 255 - _tp->val[1];
    _tp->val[2] = 255 - _tp->val[2];
  };

};

#endif //YOLOV5_ATTACK_ATTACK_PIXEL_H_
