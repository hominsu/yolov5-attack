//
// Created by Homin Su on 2021/9/6.
//

#ifndef YOLOV5_CONFIG_BASIC_SETTING_H_
#define YOLOV5_CONFIG_BASIC_SETTING_H_

#include <nlohmann/json.hpp>

#include <string>

namespace Basic {
/**
 * @brief 基础设置
 */
struct Setting {
 private:
  int sawtooth_span_; // json: "sawtooth_span"
  int interval_;      // json: "interval"
  int pixel_size_;    // json: "pixel_size"
  std::string attack_type_; // json: "attack_type"

 public:
  explicit Setting(const nlohmann::json &j)
      : sawtooth_span_(j.at("sawtooth_span")),
        interval_(j.at("interval")),
        pixel_size_(j.at("pixel_size")),
        attack_type_(j.at("attack_type")) {};

  [[nodiscard]] int sawtooth_span() const {
    return sawtooth_span_;
  }
  [[nodiscard]] int interval() const {
    return interval_;
  }
  [[nodiscard]] int pixel_size() const {
    return pixel_size_;
  }
  [[nodiscard]] std::string attack_type() const {
    return attack_type_;
  }
};
}

#endif //YOLOV5_CONFIG_BASIC_SETTING_H_
