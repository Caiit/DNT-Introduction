#!/usr/bin/env python2

import cv2
import numpy as np

from naoqi import ALProxy

from orange_ball import create_video_connection, get_img_from_robot, detect_orange_ball


IP = "EVE.local"
PORT = 9559

# Tracking method: 0: head, 1: walk to ball
TRACKING_METHOD = 1

def start_motion_and_posture_proxy():
    '''
    Start the motion and posture proxy and let the robot stand up.
    '''
    motion_proxy = ALProxy("ALMotion", IP, PORT)
    posture_proxy = ALProxy("ALRobotPosture", IP, PORT)
    motion_proxy.wakeUp()
    posture_proxy.goToPosture("StandInit", 0.5)
    motion_proxy.setAngles("HeadPitch", 0.3, 0.1)

    return motion_proxy, posture_proxy


def track_ball_with_head(motion_proxy, x, y, width, height):
    '''
    Track the ball with the robots head by changing the head yaw and pitch.
    http://doc.aldebaran.com/2-1/naoqi/motion/control-joint-api.html#ALMotionProxy::changeAngles__AL::ALValueCR.AL::ALValueCR.floatCR
    '''
    hor_dist_to_centre = x - 0.5 * width
    ver_dist_to_centre = y - 0.5 * height
    if np.abs(hor_dist_to_centre) > 15:
        motion_proxy.changeAngles("HeadYaw", -0.03*np.sign(hor_dist_to_centre), 0.1)
    if np.abs(ver_dist_to_centre) > 15:
        motion_proxy.changeAngles("HeadPitch", 0.03*np.sign(ver_dist_to_centre), 0.1)


def reset_head_to_centre(motion_proxy):
    '''
    Reset the head angles to zero.
    http://doc.aldebaran.com/2-1/family/robots/joints_robot.html
    '''
    motion_proxy.setAngles("HeadYaw", 0, 0.1)
    motion_proxy.setAngles("HeadPitch", 0, 0.1)


def move_to_ball(motion_proxy, x, y, width, height):
    '''
    Move towards the ball.
    http://doc.aldebaran.com/2-1/naoqi/motion/control-walk-api.html#ALMotionProxy::moveToward__floatCR.floatCR.floatCR
    '''
    robot_x     = 1.0
    robot_y     = 0.0
    theta       = 0.0
    hor_dist_to_centre = x - 0.5 * width
    if np.abs(hor_dist_to_centre) > 10:
        # If ball not in centre, turn towards the ball
        theta = -0.3 * np.sign(x - 0.5 * width)
    # If close to ball, slowly walk forward
    if y > height / 2 + 15: robot_x = 0.5
    motion_proxy.moveToward(robot_x, robot_y, theta)



def main():
    # Start connection with the robot
    video_device, capture_device = create_video_connection(IP, PORT, 1)
    motion_proxy, posture_proxy = start_motion_and_posture_proxy()
    while (True):
        img = get_img_from_robot(video_device, capture_device)

        x, y, r = detect_orange_ball(img)

        if x is not None:
            if TRACKING_METHOD == 0:
                track_ball_with_head(motion_proxy, x, y, img.shape[1], img.shape[0])
            elif TRACKING_METHOD == 1:
                move_to_ball(motion_proxy, x, y, img.shape[1], img.shape[0])
            # Draw the outer circle
            cv2.circle(img, (x, y), r, (0, 255, 0), 2)
            # Draw the center of the circle
            cv2.circle(img, (x, y), 2, (0, 0, 255), 3)
        else:
            if TRACKING_METHOD == 0:
                # Reset head to centre
                reset_head_to_centre(motion_proxy)
            elif TRACKING_METHOD == 1:
                # TODO: search for ball
                print("Search for ball")
        # Show image
        cv2.imshow("Red ball detector", img)

        # Quit with q
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()

    video_device.unsubscribe(capture_device)

    motion_proxy.stopMove()
    motion_proxy.rest()


if __name__ == "__main__":
    main()
