#include <dirent.h>
#include <iostream>

#include "orange_ball.h"


void OrangeBall::readImgs(std::string img_dir) {
  DIR* dirp = opendir(img_dir.c_str());
  struct dirent * dp;
  while ((dp = readdir(dirp)) != NULL) {
    cv::Mat image = cv::imread(img_dir + dp->d_name);
    if (image.data) {
      imgs_.push_back(image);
    }
  }
  closedir(dirp);
}

void OrangeBall::startWebcam() {
  cap_ = cv::VideoCapture(0);
}

void OrangeBall::getImgFromWebcam(cv::Mat& img) {
    cap_ >> img;
}

void OrangeBall::setRobotIpAndPort(std::string ip, int port) {
  IP = ip;
  PORT = port;
}

void OrangeBall::startRobotCamera() {
  cam_proxy_ = new AL::ALVideoDeviceProxy(IP, PORT);
  camera_client_ = cam_proxy_->subscribeCamera("camera", 0, AL::kQVGA, AL::kBGRColorSpace, 30);
}

void OrangeBall::stopRobotCamera() {
  cam_proxy_->unsubscribe(camera_client_);
}

void OrangeBall::getImgFromRobot(cv::Mat& img) {
  AL::ALValue al_img = cam_proxy_->getImageRemote(camera_client_);

  cv::Mat img_header = cv::Mat(cv::Size(320, 240), CV_8UC3);
  img_header.data = (uchar*) al_img[6].GetBinary();
  cam_proxy_->releaseImage(camera_client_);

  img_header.copyTo(img);
}

cv::Mat OrangeBall::maskImg(cv::Mat img) {
  cv::Scalar lower = cv::Scalar(0, 120, 120);
  cv::Scalar upper = cv::Scalar(15, 255, 255);

  cv::Mat mask;
  cv::inRange(img, lower, upper, mask);

  return mask;
}

Ball OrangeBall::detectCircles(cv::Mat img) {
  std::vector<cv::Vec3f> circles;
  // Depending on opencv version
  // cv::HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1.2, 200, 100, 5, 5, 150);
  cv::HoughCircles(img, circles, cv::HOUGH_GRADIENT, 1.2, 200, 100, 5, 5, 150);

  // The minimum amount of orange in a square
  float best_percentage_orange = 0.3;
  Ball ball;
  for (size_t i = 0; i < circles.size(); ++i) {
    // 4 Points of Region of interest square
    int x_min = std::max(0, int(circles[i][0] - circles[i][2]));
    int x_max = std::min(img.cols, int(circles[i][0] + circles[i][2]));
    int y_min = std::max(0, int(circles[i][1] - circles[i][2]));
    int y_max = std::min(img.rows, int(circles[i][1] + circles[i][2]));

    // For now just take the square instead of circle
    cv::Mat roi = img(cv::Rect(x_min, y_min, x_max - x_min, y_max - y_min));

    // Compute the amount of white pixels (thus the orange ones after applying the mask)
    float non_zero = cv::countNonZero(roi) / float(roi.rows * roi.cols);

    // Set the one with the most orange pixels on average as best circle
    if (non_zero > best_percentage_orange) {
      ball.x = circles[i][0];
      ball.y = circles[i][1];
      ball.r = circles[i][2];
      best_percentage_orange = non_zero;
    }
  }

  return ball;
}

Ball OrangeBall::detectOrangeBall(cv::Mat img) {
  cv::Mat hsv;
  // Depending on opencv version
  // cv::cvtColor(img, hsv, CV_BGR2HSV);
  cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
  cv::Mat mask = maskImg(hsv);

  Ball ball = detectCircles(mask);
  return ball;
}

void OrangeBall::detect() {
  bool next = true;
  if (camera_type_ == CameraType::folder) {
    readImgs("./../../../../../img/");
    next = imgs_.size() > 0;
  } else if (camera_type_ == CameraType::webcam) {
    // Start webcam
    startWebcam();
    if (!cap_.isOpened()) {
      // Check if webcam is open
      std::cout << "Could not open webcam" << std::endl;
      return;
    }
  } else if (camera_type_ == CameraType::robot) {
    startRobotCamera();
  }

  cv::Mat img;
  while (next) {
    // Get newest image from device
    if (camera_type_ == CameraType::folder) {
      img = imgs_.back();
      imgs_.pop_back();
      next = imgs_.size() > 0;
    } else if (camera_type_ == CameraType::webcam) {
      getImgFromWebcam(img);
    } else if (camera_type_ == CameraType::robot) {
      getImgFromRobot(img);
    }

    // Run ball detector
    Ball ball = detectOrangeBall(img);
    // Draw ball if detected
    if (!ball.r == std::numeric_limits<int>::infinity()) {
      cv::circle(img, cv::Point(ball.x, ball.y), ball.r, cv::Scalar(0, 255, 0), 2);
      cv::circle(img, cv::Point(ball.x, ball.y), 2, cv::Scalar(0, 0, 255), 3);
    }

    // Show image
    cv::imshow("Orange ball detector", img);
    if (camera_type_ == CameraType::folder) {
      // Press any key to go to the next img
      cv::waitKey(0);
    } else {
      // Quit with esc, Q or q
      int key = cv::waitKey(1);
      if (key == 27 || key == 'Q' || key == 'q') break;
    }
  }

  cv::destroyAllWindows();

  if (camera_type_ == CameraType::webcam) {
    cap_.release();
  } else if (camera_type_ == CameraType::robot) {
    stopRobotCamera();
  }
}
