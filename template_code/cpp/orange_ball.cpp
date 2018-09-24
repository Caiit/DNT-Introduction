#include <dirent.h>
#include <iostream>

#include "orange_ball.h"

void OrangeBall::readImgs(std::string img_dir) {
  // Read image from folder and store in imgs_
}

void OrangeBall::startWebcam() {
  // Start the webcam
}

void OrangeBall::getImgFromWebcam(cv::Mat& img) {
  // Put the new frame in img
}

void OrangeBall::setRobotIpAndPort(std::string ip, int port) {
  // Set ip and port
}

void OrangeBall::startRobotCamera() {
  // Start robot camera
}

void OrangeBall::stopRobotCamera() {
  // Stop robot camera
}

void OrangeBall::getImgFromRobot(cv::Mat& img) {
  // Put the new frame in img
}

cv::Mat OrangeBall::maskImg(cv::Mat img) {
  cv::Mat mask;
  // Mask the image
  return mask;
}

Ball OrangeBall::detectCircles(cv::Mat img) {
  Ball ball;
  // Detect circles
  return ball;
}

Ball OrangeBall::detectOrangeBall(cv::Mat img) {
  Ball ball;
  // Convert to HSV
  // Apply mask
  // Detect circles
  return ball;
}

void OrangeBall::detect() {
  // Depending on the camera type (folder, webcam, robot)
  // Loop over the images, detect the ball and show the results
  std::cout << "Good luck" << std::endl;
}
