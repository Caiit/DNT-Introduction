#include "ball_tracker.h"

void BallTracker::startProxies() {
  // Start proxies and let the robots stand up
}

void BallTracker::trackBallWithHead(int x, int y, int width, int height) {
  // Move head left or right depending on the x position of the ball (HeadYaw)
  // Move head up or down depending on the y position of the ball (HeadPitch)
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
