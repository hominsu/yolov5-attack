//
// Created by Homin Su on 2021/9/6.
//

#ifndef YOLOV5_CONFIG_BASIC_SETTING_H_
#define YOLOV5_CONFIG_BASIC_SETTING_H_

#include <nlohmann/json.hpp>

namespace Basic {
/**
 * @brief 基础设置
 */
struct Setting {
 private:
  int sawtooth_span_; // json: "sawtooth_span"
  int interval_;      // json: "interval"
  int pixel_size_;    // json: "pixel_size"

 public:
  explicit Setting(const nlohmann::json &j)
      : sawtooth_span_(j.at("sawtooth_span")), interval_(j.at("interval")), pixel_size_(j.at("pixel_size")) {};

  [[nodiscard]] int sawtooth_span() const {
    return sawtooth_span_;
  }
  [[nodiscard]] int interval() const {
    return interval_;
  }
  [[nodiscard]] int pixel_size() const {
    return pixel_size_;
  }
};
}

#endif //YOLOV5_CONFIG_BASIC_SETTING_H_
