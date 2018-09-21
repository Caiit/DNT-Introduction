#include <dirent.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "orange_ball.h"

std::vector<cv::Mat> OrangeBall::readImgs(std::string img_dir) {
  std::vector<cv::Mat> imgs;

  DIR* dirp = opendir(img_dir.c_str());
  struct dirent * dp;
  while ((dp = readdir(dirp)) != NULL) {
    cv::Mat image = cv::imread(img_dir + dp->d_name);
    if (image.data) {
      imgs.push_back(image);
    }
  }
  closedir(dirp);

  return imgs;
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

/**************************************************/

int main() {
  OrangeBall orange_ball;
  std::vector<cv::Mat> imgs = orange_ball.readImgs("../../../img/");

  for (cv::Mat img : imgs) {
    Ball ball = orange_ball.detectOrangeBall(img);
    if (!ball.r == std::numeric_limits<int>::infinity()) {
      cv::circle(img, cv::Point(ball.x, ball.y), ball.r, cv::Scalar(0, 255, 0), 2);
      cv::circle(img, cv::Point(ball.x, ball.y), 2, cv::Scalar(0, 0, 255), 3);
    }

    cv::imshow("Red ball detector", img);
    cv::waitKey(0);
  }

  return 0;
}
