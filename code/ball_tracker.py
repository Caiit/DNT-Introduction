#!/usr/bin/env python2

import cv2
import numpy as np

from naoqi import ALProxy

from orange_ball import create_video_connection, get_img_from_robot, detect_orange_ball


IP = "EVE.local"
PORT = 9559

def start_motion_proxy():
    '''
    Start the motion proxy.
    http://doc.aldebaran.com/2-4/dev/python/examples/motion/walk.html
    '''
    motion_proxy = ALProxy("ALMotion", IP, PORT)
    motion_proxy.setStiffnesses("Head", 1.0)
    return motion_proxy


def track_ball_with_head(motion_proxy, x, y, width, height):
    '''
    Track the ball with the robots head by changing the head yaw and pitch.
    http://doc.aldebaran.com/2-1/naoqi/motion/control-joint-api.html#ALMotionProxy::changeAngles__AL::ALValueCR.AL::ALValueCR.floatCR
    '''
    hor_dist_to_centre = x - 0.5 * width
    ver_dist_to_centre = y - 0.5 * height
    if np.abs(hor_dist_to_centre) > 10:
        motion_proxy.changeAngles("HeadYaw", -0.03*np.sign(hor_dist_to_centre), 0.1)
    if np.abs(ver_dist_to_centre) > 10:
        motion_proxy.changeAngles("HeadPitch", 0.03*np.sign(ver_dist_to_centre), 0.1)


def head_to_centre(motion_proxy):
    '''
    Reset the head angles to zero.
    http://doc.aldebaran.com/2-1/family/robots/joints_robot.html
    '''
    motion_proxy.setAngles("HeadYaw", 0, 0.1)
    motion_proxy.setAngles("HeadPitch", 0, 0.1)


def main():
    # Start connection with the robot
    video_device, capture_device = create_video_connection(IP, PORT)
    motion_proxy = start_motion_proxy()
    while (True):
        img = get_img_from_robot(video_device, capture_device)

        x, y, r = detect_orange_ball(img)

        if x is not None:
            track_ball_with_head(motion_proxy, x, y, img.shape[1], img.shape[0])
            # Draw the outer circle
            cv2.circle(img, (x, y), r, (0, 255, 0), 2)
            # Draw the center of the circle
            cv2.circle(img, (x, y), 2, (0, 0, 255), 3)
        else:
            # Reset head
            head_to_centre(motion_proxy)
        # Show image
        cv2.imshow("Red ball detector", img)

        # Quit with q
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()

    video_device.unsubscribe(capture_device)
    motion_proxy.setStiffnesses("Head", 0.0)


if __name__ == "__main__":
    main()
