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

  void trackBallWithHead(int x, int y, int width, int height);


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
  TrackingMethod tracking_method_ = TrackingMethod::head;

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
};
