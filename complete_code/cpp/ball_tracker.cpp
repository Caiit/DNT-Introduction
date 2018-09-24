#include "ball_tracker.h"

void BallTracker::startProxies() {
  motion_proxy_ = new AL::ALMotionProxy(IP, PORT);
  posture_proxy_ = new AL::ALRobotPostureProxy(IP, PORT);
  memory_proxy_ = new AL::ALMemoryProxy(IP, PORT);

  motion_proxy_->wakeUp();
  posture_proxy_->goToPosture("StandInit", 0.5);
  motion_proxy_->setAngles("HeadPitch", 0.3, 0.1);
}

void BallTracker::trackBallWithHead(int x, int y, int width, int height) {
  float hor_dist_to_centre = x - 0.5 * width;
  float ver_dist_to_centre = y - 0.5 * height;
  if (std::abs(hor_dist_to_centre) > 15) {
    motion_proxy_->changeAngles("HeadYaw", 0.03*std::signbit(hor_dist_to_centre), 0.1);
  }
  if (std::abs(ver_dist_to_centre) > 15) {
    motion_proxy_->changeAngles("HeadPitch", -0.03*std::signbit(ver_dist_to_centre), 0.1);
  }
}

void BallTracker::track() {
  OrangeBall ball_detector;
  ball_detector.setRobotIpAndPort(IP, PORT);
  ball_detector.startRobotCamera();
  startProxies();
  cv::Mat img;
  while (true) {
    ball_detector.getImgFromRobot(img);
    Ball ball = ball_detector.detectOrangeBall(img);

    // If ball found
    if (!ball.r == std::numeric_limits<int>::infinity()) {
      if (tracking_method_ == TrackingMethod::head) {
        trackBallWithHead(ball.x, ball.y, img.cols, img.rows);
      }
      // Draw circles on the ball
      cv::circle(img, cv::Point(ball.x, ball.y), ball.r, cv::Scalar(0, 255, 0), 2);
      cv::circle(img, cv::Point(ball.x, ball.y), 2, cv::Scalar(0, 0, 255), 3);
    }
    // Show image
    cv::imshow("Orange ball detector", img);
    // Quit with esc, Q or q
    int key = cv::waitKey(1);
    if (key == 27 || key == 'Q' || key == 'q') break;
  }

  cv::destroyAllWindows();
  ball_detector.stopRobotCamera();

  // Let the robot sit when done
  motion_proxy_->stopMove();
  motion_proxy_->rest();
}
