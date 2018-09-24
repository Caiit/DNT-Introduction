#!/usr/bin/env python2

import cv2
import glob
import numpy as np
import os

from naoqi import ALProxy


# Robot name as shown on its head followed by .local
IP = "EVE.local"
PORT = 9559

# 0: images from folder, 1: webcam, 2: robot
CAMERA_TYPE = 0

def read_imgs(img_dir):
    '''
    Read images from the given folder and return them in a list.
    https://www.quora.com/How-can-I-read-multiple-images-in-Python-presented-in-a-folder
    '''
    pass
    return imgs


def start_webcam():
    '''
    Start the webcam and return the capture device.
    https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_gui/py_video_display/py_video_display.html
    '''
    pass
    return cap


def get_img_from_webcam(cap):
    '''
    Get a frame from the webcam and return it.
    https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_gui/py_video_display/py_video_display.html
    '''
    pass
    return frame


def close_webcam(cap):
    '''
    Stop the connection with the webcam.
    https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_gui/py_video_display/py_video_display.html
    '''
    pass


def create_video_connection(ip=None, port=None, camera=0):
    '''
    Create a connection with the robot and start a camera proxy.
    returns the video device and the capture device.
    https://gist.github.com/takamin/990aa0133919aa58944d
    '''
    if ip == None: ip = IP
    if port == None: port = PORT
    pass
    return video_device, capture_device


def get_img_from_robot(video_device, capture_device):
    '''
    Get a frame from the robot and return it as numpy array.
    https://gist.github.com/takamin/990aa0133919aa58944d
    '''
    # Create image
    pass
    return img


def mask_img(img):
    '''
    Create a binary image in which orange is white and the rest is black.
    https://www.pyimagesearch.com/2014/08/04/opencv-python-color-detection/
    '''
    pass
    return mask


def detect_circles(img):
    '''
    Detect circles in a binary image.
    returns the centre coordinates and the radius of the ball in the image.
    https://www.pyimagesearch.com/2014/07/21/detecting-circles-images-using-opencv-hough-circles/
    '''
    pass
    return x, y, r


def detect_orange_ball(img):
    '''
    Detect an orange ball in the given image.
    returns the centre coordinates and the radius of the ball in the image.
    '''
    pass
    # Convert to HSV
    # Apply mask
    # Detect circles
    return x, y, r


def main():
    # Depending on the camera type (folder, webcam, robot)
    # Loop over the images, detect the ball and show the results
    print("Good luck")



if __name__ == "__main__":
    main()
