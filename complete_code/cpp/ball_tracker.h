#pragma once

// Aldebaran includes.
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>
#include <alproxies/almemoryproxy.h>

#include "orange_ball.h"

enum class TrackingMethod {
  head,
  walk,
  kick
};

class BallTracker {
public:
  /**
   * Constructor: start the proxies
   */
  BallTracker() = default;

  /**
   * Start tracking the ball.
   * Depending on tracking method with its head, by walking towards it, or kick.
   */
  void track();

private:
  /**
   * Start the motion, posture and memory proxies and let the robot stand up.
   */
  void startProxies();

  /**
   * Move the head such that the ball is in the centre of the image.
   */
  void trackBallWithHead(int x, int y, int width, int height);

  /**
   * Move towards the ball.
   */
  void moveToBall(int x, int y, int width, int height);

  /**
   * Consider kicking, if close enough to the ball, kick, otherwise walk to the ball.
   */
  void considerKicking(int x, int y, int width, int height);

  /**
   * Search for the ball by moving the head in a pattern.
   */
  void searchForBall();

  /**
   * Perform kick motion.
   */
  void kick(bool side);

  /**
  * IP of the robot.
  * Robot name as shown on its head followed by .local
  */
  std::string IP = "EVE.local";

  /**
  * Port of the robot.
  */
  int PORT = 9559;

  /**
   * Specify the tranking method
   */
  TrackingMethod tracking_method_ = TrackingMethod::kick;

  /**
   * The motion proxy.
   */
  AL::ALMotionProxy* motion_proxy_;

  /**
   * The posture proxy.
   */
  AL::ALRobotPostureProxy* posture_proxy_;

  /**
   * The memory proxy.
   */
  AL::ALMemoryProxy* memory_proxy_;

  /**
   * Amount of times that the ball is not seen.
   */
  int times_no_ball_ = 0;

  /**
   * Index of the head search.
   */
  int head_search_index_ = 0;

  float head_search_yaw_[9]   = {0.f, -1.f,  -2.f,  -1.f,  0.f, 1.f,  2.f,  1.f,  0.f};
  float head_search_pitch_[9] = {0.f, -0.3f, -0.5f, -0.3f, 0.f, 0.3f, 0.5f, 0.3f, 0.f};
};
