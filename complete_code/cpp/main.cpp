#include "orange_ball.h"
#include "ball_tracker.h"

enum class Method {
  ball_detector,
  ball_tracker
};

int main() {
  Method method = Method::ball_detector;
  if (method == Method::ball_detector) {
    OrangeBall orange_ball;
    orange_ball.detect();
  } else if (method == Method::ball_tracker) {
    BallTracker ball_tracker;
    ball_tracker.track();
  }
  return 0;
}
