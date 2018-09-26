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
    int direction = std::signbit(hor_dist_to_centre) ? 1 : -1;
    motion_proxy_->changeAngles("HeadYaw", 0.03*direction, 0.1);
  }
  if (std::abs(ver_dist_to_centre) > 15) {
    int direction = std::signbit(ver_dist_to_centre) ? 1 : -1;
    motion_proxy_->changeAngles("HeadPitch", -0.03*direction, 0.1);
  }
}

void BallTracker::moveToBall(int x, int y, int width, int height) {
  float robot_x = 1.f;
  float robot_y = 0.f;
  float theta = 0.f;

  float hor_dist_to_centre = x - 0.5 * width;
  if (std::abs(hor_dist_to_centre) > 10) {
    // If ball not in centre, turn towards the ball
    int dir = std::signbit(x - 0.5 * width) ? 1 : -1;
    theta = -0.3 * dir;
  }
  if (y > (height / 2 + 15)) {
    // If close to ball, walk slower
    robot_x = 0.5;
  }
  motion_proxy_->moveToward(robot_x, robot_y, theta);
}

void BallTracker::considerKicking(int x, int y, int width, int height) {
  if ((y > height - 50) && (std::abs(x - 0.5 * width) < 25)) {
    // If ball is close enough and in front of the robot
    // Check if its head is correct an kick the ball
    float pitch_value = memory_proxy_->getData("Device/SubDeviceList/HeadPitch/Position/Sensor/Value");
    float yaw_value = memory_proxy_->getData("Device/SubDeviceList/HeadYaw/Position/Sensor/Value");
    if (pitch_value < 0.27 || pitch_value > 0.33) {
      motion_proxy_->setAngles("HeadPitch", 0.3, 0.1);
    }
    else if (yaw_value < -0.1 or yaw_value > 0.1) {
      motion_proxy_->setAngles("HeadYaw", 0.0, 0.1);
    }
    else {
      std::cout << "KICK" << std::endl;
      // TODO: add kick file
    }
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
      } else if (tracking_method_ == TrackingMethod::walk) {
        moveToBall(ball.x, ball.y, img.cols, img.rows);
      } else if (tracking_method_ == TrackingMethod::kick) {
        considerKicking(ball.x, ball.y, img.cols, img.rows);
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
