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
    theta = 0.3 * dir;
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
    // Check if its head is correct to kick the ball
    float pitch_value = memory_proxy_->getData("Device/SubDeviceList/HeadPitch/Position/Sensor/Value");
    float yaw_value = memory_proxy_->getData("Device/SubDeviceList/HeadYaw/Position/Sensor/Value");
    if (pitch_value < 0.27 || pitch_value > 0.33) {
      motion_proxy_->setAngles("HeadPitch", 0.3, 0.1);
    }
    else if (yaw_value < -0.1 or yaw_value > 0.1) {
      motion_proxy_->setAngles("HeadYaw", 0.0, 0.1);
    }
    else {
      bool side = std::signbit(x - 0.5 * width) ? false : true;
      motion_proxy_->stopMove();
      kick(side);
    }
  } else {
    // Ball is not close or in right position
    moveToBall(x, y, width, height);
  }
}

void BallTracker::searchForBall() {
  motion_proxy_->stopMove();
  motion_proxy_->setAngles("HeadYaw", head_search_yaw_[head_search_index_], 0.1);
  motion_proxy_->setAngles("HeadPitch", head_search_pitch_[head_search_index_], 0.1);
  ++head_search_index_;
  // TODO: not hardcode the 8
  if (head_search_index_ > 8) {
    head_search_index_ = 0;
  }
}

void BallTracker::track() {
  OrangeBall ball_detector;
  ball_detector.setRobotIpAndPort(IP, PORT);
  ball_detector.startRobotCamera(1);
  startProxies();
  cv::Mat img;
  while (true) {
    ball_detector.getImgFromRobot(img);
    Ball ball = ball_detector.detectOrangeBall(img);

    // If ball found
    if (!ball.r == std::numeric_limits<int>::infinity()) {
      // Resert search for ball variables
      times_no_ball_ = 0;
      head_search_index_ = 0;
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
    } else {
      // No ball found
      if (times_no_ball_ > 5) {
        searchForBall();
      }
      ++times_no_ball_;
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

void BallTracker::kick(bool side=false) {
    posture_proxy_->goToPosture("StandInit", 0.5);
    bool isAbsolute = true;

    if (side) {
      // Kick right
      std::vector<std::string> names;
      names.reserve(14);
      names.push_back("RShoulderRoll");
      names.push_back("RShoulderPitch");
      names.push_back("LShoulderRoll");
      names.push_back("LShoulderPitch");
      names.push_back("RHipRoll");
      names.push_back("RHipPitch");
      names.push_back("RKneePitch");
      names.push_back("RAnklePitch");
      names.push_back("RAnkleRoll");
      names.push_back("LHipRoll");
      names.push_back("LHipPitch");
      names.push_back("LKneePitch");
      names.push_back("LAnklePitch");
      names.push_back("LAnkleRoll");

      AL::ALValue angleLists;
      angleLists.arraySetSize(14);
      angleLists[0]  = AL::ALValue::array(-0.3f);
      angleLists[1]  = AL::ALValue::array(0.4f);
      angleLists[2]  = AL::ALValue::array(1.0f);
      angleLists[3]  = AL::ALValue::array(1.0f);
      angleLists[4]  = AL::ALValue::array(0.0f);
      angleLists[5]  = AL::ALValue::array(-0.4f, -0.2f);
      angleLists[6]  = AL::ALValue::array(0.95f, 1.5f);
      angleLists[7]  = AL::ALValue::array(-0.55f, -1.0f);
      angleLists[8]  = AL::ALValue::array(0.2f);
      angleLists[9]  = AL::ALValue::array(0.0f);
      angleLists[10] = AL::ALValue::array(-0.4f);
      angleLists[11] = AL::ALValue::array(0.95f);
      angleLists[12] = AL::ALValue::array(-0.55f);
      angleLists[13] = AL::ALValue::array(0.2f);

      AL::ALValue timeLists;
      timeLists.arraySetSize(14);
      timeLists[0]  = AL::ALValue::array(0.5f);
      timeLists[1]  = AL::ALValue::array(0.5f);
      timeLists[2]  = AL::ALValue::array(0.5f);
      timeLists[3]  = AL::ALValue::array(0.5f);
      timeLists[4]  = AL::ALValue::array(0.5f);
      timeLists[5]  = AL::ALValue::array(0.4f, 0.8f);
      timeLists[6]  = AL::ALValue::array(0.4f, 0.8f);
      timeLists[7]  = AL::ALValue::array(0.4f, 0.8f);
      timeLists[8]  = AL::ALValue::array(0.4f);
      timeLists[9]  = AL::ALValue::array(0.5f);
      timeLists[10] = AL::ALValue::array(0.4f);
      timeLists[11] = AL::ALValue::array(0.4f);
      timeLists[12] = AL::ALValue::array(0.4f);
      timeLists[13] = AL::ALValue::array(0.4f);

      motion_proxy_->angleInterpolation(names, angleLists, timeLists, isAbsolute);

      AL::ALValue names_alvalue = AL::ALValue::array("RShoulderRoll", "RHipPitch", "RKneePitch", "RAnklePitch", "LShoulderPitch");
      angleLists.clear();
      angleLists.arraySetSize(5);
      angleLists[0]  = AL::ALValue::array(1.0f);
      angleLists[1]  = AL::ALValue::array(-0.7f);
      angleLists[2]  = AL::ALValue::array(1.05f);
      angleLists[3]  = AL::ALValue::array(-0.5f);
      angleLists[4]  = AL::ALValue::array(0.2f);

      timeLists.clear();
      timeLists.arraySetSize(5);
      timeLists[0]  = AL::ALValue::array(0.1f);
      timeLists[1]  = AL::ALValue::array(0.1f);
      timeLists[2]  = AL::ALValue::array(0.1f);
      timeLists[3]  = AL::ALValue::array(0.1f);
      timeLists[4]  = AL::ALValue::array(0.1f);
      motion_proxy_->angleInterpolation(names_alvalue, angleLists, timeLists, isAbsolute);

      names_alvalue = AL::ALValue::array("RHipPitch","RKneePitch", "RAnklePitch");
      angleLists.clear();
      angleLists.arraySetSize(3);
      angleLists[0]  = AL::ALValue::array(-0.5f);
      angleLists[1]  = AL::ALValue::array(1.1f);
      angleLists[2]  = AL::ALValue::array(-0.65f);

      timeLists.clear();
      timeLists.arraySetSize(3);
      timeLists[0]  = AL::ALValue::array(0.25f);
      timeLists[1]  = AL::ALValue::array(0.25f);
      timeLists[2]  = AL::ALValue::array(0.25f);
      motion_proxy_->angleInterpolation(names_alvalue, angleLists, timeLists, isAbsolute);
    } else {
      // Kick right
      std::vector<std::string> names;
      names.reserve(14);
      names.push_back("LShoulderRoll");
      names.push_back("LShoulderPitch");
      names.push_back("RShoulderRoll");
      names.push_back("RShoulderPitch");
      names.push_back("LHipRoll");
      names.push_back("LHipPitch");
      names.push_back("LKneePitch");
      names.push_back("LAnklePitch");
      names.push_back("LAnkleRoll");
      names.push_back("RHipRoll");
      names.push_back("RHipPitch");
      names.push_back("RKneePitch");
      names.push_back("RAnklePitch");
      names.push_back("RAnkleRoll");

      AL::ALValue angleLists;
      angleLists.arraySetSize(14);
      angleLists[0]  = AL::ALValue::array(0.3f);
      angleLists[1]  = AL::ALValue::array(0.4f);
      angleLists[2]  = AL::ALValue::array(-0.5f);
      angleLists[3]  = AL::ALValue::array(1.0f);
      angleLists[4]  = AL::ALValue::array(0.0f);
      angleLists[5]  = AL::ALValue::array(-0.4f, -0.2f);
      angleLists[6]  = AL::ALValue::array(0.95f, 1.5f);
      angleLists[7]  = AL::ALValue::array(-0.55f, -1.0f);
      angleLists[8]  = AL::ALValue::array(-0.2f);
      angleLists[9]  = AL::ALValue::array(0.0f);
      angleLists[10] = AL::ALValue::array(-0.4f);
      angleLists[11] = AL::ALValue::array(0.95f);
      angleLists[12] = AL::ALValue::array(-0.55f);
      angleLists[13] = AL::ALValue::array(-0.2f);

      AL::ALValue timeLists;
      timeLists.arraySetSize(14);
      timeLists[0]  = AL::ALValue::array(0.5f);
      timeLists[1]  = AL::ALValue::array(0.5f);
      timeLists[2]  = AL::ALValue::array(0.5f);
      timeLists[3]  = AL::ALValue::array(0.5f);
      timeLists[4]  = AL::ALValue::array(0.5f);
      timeLists[5]  = AL::ALValue::array(0.4f, 0.8f);
      timeLists[6]  = AL::ALValue::array(0.4f, 0.8f);
      timeLists[7]  = AL::ALValue::array(0.4f, 0.8f);
      timeLists[8]  = AL::ALValue::array(0.4f);
      timeLists[9]  = AL::ALValue::array(0.5f);
      timeLists[10] = AL::ALValue::array(0.4f);
      timeLists[11] = AL::ALValue::array(0.4f);
      timeLists[12] = AL::ALValue::array(0.4f);
      timeLists[13] = AL::ALValue::array(0.4f);

      motion_proxy_->angleInterpolation(names, angleLists, timeLists, isAbsolute);

      AL::ALValue names_alvalue = AL::ALValue::array("LShoulderRoll", "LHipPitch", "LKneePitch",
                                                     "LAnklePitch");
      angleLists.clear();
      angleLists.arraySetSize(4);
      angleLists[0]  = AL::ALValue::array(1.0f);
      angleLists[1]  = AL::ALValue::array(-0.7f);
      angleLists[2]  = AL::ALValue::array(1.05f);
      angleLists[3]  = AL::ALValue::array(-0.5f);

      timeLists.clear();
      timeLists.arraySetSize(4);
      timeLists[0]  = AL::ALValue::array(0.1f);
      timeLists[1]  = AL::ALValue::array(0.1f);
      timeLists[2]  = AL::ALValue::array(0.1f);
      timeLists[3]  = AL::ALValue::array(0.1f);
      motion_proxy_->angleInterpolation(names_alvalue, angleLists, timeLists, isAbsolute);

      names_alvalue = AL::ALValue::array("LHipPitch","LKneePitch", "LAnklePitch");
      angleLists.clear();
      angleLists.arraySetSize(3);
      angleLists[0]  = AL::ALValue::array(-0.5f);
      angleLists[1]  = AL::ALValue::array(1.1f);
      angleLists[2]  = AL::ALValue::array(-0.65f);

      timeLists.clear();
      timeLists.arraySetSize(3);
      timeLists[0]  = AL::ALValue::array(0.25f);
      timeLists[1]  = AL::ALValue::array(0.25f);
      timeLists[2]  = AL::ALValue::array(0.25f);
      motion_proxy_->angleInterpolation(names_alvalue, angleLists, timeLists, isAbsolute);
    }


    posture_proxy_->goToPosture("StandInit", 0.5);
    motion_proxy_->setAngles("HeadPitch", 0.3, 0.1);
}
