//
// Created by Homin Su on 2021/9/3.
//

#include <thread>
#include <csignal>
#include <filesystem>

#include <opencv4/opencv2/opencv.hpp>

#include "model/yolo_v_5.h"
#include "model/gen_color.h"
#include "attack/attack.h"

bool is_running = true;

// signal call back function
void OnSignal(int) {
  is_running = false;
}

int main(int argc, char **argv) {

  signal(SIGINT, OnSignal);
  signal(SIGQUIT, OnSignal);
  signal(SIGTERM, OnSignal);

  // Process Exception from Parse、Gen engine、Load engine
  try {
    YoloV5::Get()->ParseArgs(argc, argv);
    YoloV5::Get()->GenEngine();
    YoloV5::Get()->LoadEngine();
  } catch (YoloQuit &_d) {
    std::cout << _d.what();
    return EXIT_SUCCESS;
  } catch (YoloException &_e) {
    std::cerr << _e.what();
    return EXIT_FAILURE;
  }

  // prepare net and gpu buffer
  YoloV5::Get()->PrepareInput();

  // rect color
  std::vector<cv::Scalar> colors_list = GetColors(YoloV5::CLASS_NUM);
  std::vector<std::string> id_name = {
      "person"
  };

  // Prepare buffer
  float data[BATCH_SIZE * 3 * YoloV5::INPUT_H * YoloV5::INPUT_W];
  float prob[BATCH_SIZE * YoloV5::OUTPUT_SIZE];


  // create output directories
  std::filesystem::create_directories("/attack_img");

  // img list
  std::list<std::filesystem::directory_entry> images_path_list_it;

  for (auto &it: std::filesystem::directory_iterator("/images")) {
    // only process regular file
    if (!it.is_regular_file()) {
      continue;
    }

    // append the file path into list
    images_path_list_it.push_back(it);
  }

  Attack attack;

  // create a window to show the detected frame
  cv::namedWindow("dst", cv::WINDOW_AUTOSIZE);

  while (true) {
    auto image_it = images_path_list_it.front();
    images_path_list_it.pop_front();

    cv::Mat img = cv::imread(image_it.path().string());
    cv::Mat src = img.clone();
    if (img.empty()) {
      continue;
    }

    // convert
    img = YoloV5::BGR2RGB(data, img);

    // Do inference
    auto boxes = YoloV5::Get()->Inference(colors_list, id_name, data, prob, img);

    // process inference result
    if (!boxes->empty()) {
      // attack
      attack.Write(boxes, src);
      cv::imwrite("/attack_img/" + image_it.path().filename().string(), src);
    }

    // show image
    cv::imshow("dst", img);
    if (cv::waitKey(1) == 'q') {
      cv::destroyAllWindows();
      break;
    }

    if (!is_running) {
      break;
    }

  }

  return EXIT_SUCCESS;
}

