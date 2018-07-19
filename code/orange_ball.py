#!/usr/bin/env python3

import cv2
import glob
import numpy as np
import os


WEBCAM = False

def read_imgs(img_dir):
    '''
    Read images from the given folder and return them as list.
    https://www.quora.com/How-can-I-read-multiple-images-in-Python-presented-in-a-folder
    '''
    img_path = os.path.join(img_dir, '*g')
    files = glob.glob(img_path)
    imgs = []
    for f in files:
        img = cv2.imread(f)
        imgs.append(img)
    return imgs


def start_webcam():
    '''
    Start the webcam.
    https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_gui/py_video_display/py_video_display.html
    '''
    return cv2.VideoCapture(0)


def get_img_from_webcam(cap):
    '''
    Get a frame from the webcam.
    https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_gui/py_video_display/py_video_display.html
    '''
    ret, frame = cap.read()
    return frame


def close_webcam(cap):
    '''
    Stop the connection with the webcam.
    https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_gui/py_video_display/py_video_display.html
    '''
    cap.release()


def mask_img(img):
    '''
    Create a binary image in which orange is white and the rest is black.
    https://www.pyimagesearch.com/2014/08/04/opencv-python-color-detection/
    '''
    # Lower and upper boundary of orange
    lower = np.array([0, 120, 120], np.uint8)
    upper = np.array([15, 255, 255], np.uint8)

    # Create a mask for orange
    mask = cv2.inRange(img, lower, upper)
    return mask


def detect_circles(img):
    '''
    Detect circles in a binary image.
    https://www.pyimagesearch.com/2014/07/21/detecting-circles-images-using-opencv-hough-circles/
    '''
    circles = cv2.HoughCircles(img, cv2.HOUGH_GRADIENT, 1.2, 200, param1=100, param2=5, minRadius=5,
                               maxRadius=150)

    # The minimum amount of orange in a square
    best_percentage_orange = 0.3
    x = None
    y = None
    r = None
    if circles is not None:
        # Draw the circles for debug
        circles = np.int16(np.around(circles))
        for circle in circles[0, :]:
            # 4 Points of Region of interest square
            x_min = circle[0] - circle[2]
            x_max = circle[0] + circle[2]
            y_min = circle[1] - circle[2]
            y_max = circle[1] + circle[2]

            # For now just take the square instead of circle
            roi = img[y_min:y_max, x_min:x_max]

            if roi.shape[0] > 0 and roi.shape[1] > 0:
                # Compute the amount of white pixels (thus the orange ones after applying the mask)
                non_zero = cv2.countNonZero(roi) / float(roi.shape[0] * roi.shape[1])

                # Set the one with the most orange pixels on average as best circle
                if non_zero > best_percentage_orange:
                    x = circle[0]
                    y = circle[1]
                    r = circle[2]
                    best_percentage_orange = non_zero

    return (x, y, r)


def detect_orange_ball(img):
    '''
    Detect an orange ball in the given image.
    return the centre coordinates of the ball in the image.
    '''
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = mask_img(hsv)

    (x, y, r) = detect_circles(mask)

    return x, y, r


def main():
    if WEBCAM:
        # Start the webcam
        cap = start_webcam()
        next = True
    else:
        # Read images from folder
        img_dir = "../img/"
        imgs = read_imgs(img_dir)
        next = len(imgs) > 0

    while (next):
        if WEBCAM:
            img = get_img_from_webcam(cap)
        else:
            img = imgs.pop()
            next = len(imgs) > 0

        x, y, r = detect_orange_ball(img)

        if x is not None:
            # Draw the outer circle
            cv2.circle(img, (x, y), r, (0, 255, 0), 2)
            # Draw the center of the circle
            cv2.circle(img, (x, y), 2, (0, 0, 255), 3)
        # Show image
        cv2.imshow("Red ball detector", img)
        if WEBCAM:
            # Quit with q
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        else:
            # Press any key to go to the next img
            cv2.waitKey(0)

    cv2.destroyAllWindows()

    if WEBCAM:
        close_webcam(cap)


if __name__ == "__main__":
    main()
