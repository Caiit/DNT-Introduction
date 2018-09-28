#include "ball_tracker.h"

void BallTracker::startProxies() {
  // Start proxies and let the robots stand up
}

void BallTracker::trackBallWithHead(int x, int y, int width, int height) {
  // Move head left or right depending on the x position of the ball (HeadYaw)
  // Move head up or down depending on the y position of the ball (HeadPitch)
}

void BallTracker::moveToBall(int x, int y, int width, int height) {
  // Turn towards the ball if not in the centre of the image
  // Walk towards the ball (moveToward)
}

void BallTracker::considerKicking(int x, int y, int width, int height) {
  // Check if ball is close enough and in front of the robot, if so kick
  // Otherwise just walk towards the ball
}

void BallTracker::searchForBall() {
  // Search for the ball by moving the robots head
}

void BallTracker::track() {
  // Start connections with the robot
  // Obtain image from the robot and detect the ball
  // Depending on TRACKING_METHOD, track the ball with the robots head
  // or by walking towards the ball
  // Search for the ball if no ball is found

  std::cout << "Good luck" << std::endl;

  // TODO: Remove this return when motion proxy exists
  return;
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
