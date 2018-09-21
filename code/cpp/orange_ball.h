#include <limits>

// Aldebaran includes.
#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alvisiondefinitions.h>

// Opencv includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct Ball {
  /**
   * x coordinate of the ball
   */
  int x;
  /**
   * y coordinate of the ball
   */
  int y;
  /**
   * Radius of the ball
   */
  int r = std::numeric_limits<int>::infinity();
};

enum class CameraType {
  folder,
  webcam,
  robot
};

class OrangeBall {
public:
  /**
   * Constructor: does nothing.
   */
  OrangeBall() = default;

  /**
   * Read images from a directory.
   * http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
   * https://docs.opencv.org/2.4/doc/tutorials/introduction/display_image/display_image.html
   */
  std::vector<cv::Mat> readImgs(std::string img_dir);

  /**
   * Start the webcam.
   * https://docs.opencv.org/3.0-beta/modules/videoio/doc/reading_and_writing_video.html
   */
  cv::VideoCapture startWebcam();

  /**
   * Get a frame from the webcam.
   * https://docs.opencv.org/3.0-beta/modules/videoio/doc/reading_and_writing_video.html
   */
  void getImgFromWebcam(cv::VideoCapture cap, cv::Mat& img);

  /**
   * Create a connection with the robot and start a camera proxy.
   * http://doc.aldebaran.com/2-4/dev/cpp/examples/vision/getimage/getimage.html
   */
  AL::ALVideoDeviceProxy* startRobotCamera(std::string ip, int port);

  /**
   * Get frame from the robot.
   * http://doc.aldebaran.com/2-4/dev/cpp/examples/vision/getimage/getimage.html
   */
  void getImgFromRobot(AL::ALVideoDeviceProxy* cam_proxy, std::string camera_client, cv::Mat img);

  /**
   * Detect an orange ball in the given image.
   */
  Ball detectOrangeBall(cv::Mat img);

  /**
   * Specify which camera needs to be used.
   */
  CameraType camera_type_ = CameraType::robot;

  /**
   * IP of the robot.
   */
   std::string IP = "localhost";

   /**
    * Port of the robot.
    */
   int PORT = 46575;


private:
  /**
   * Create a binary image in which orange is white and the rest is black.
   */
  cv::Mat maskImg(cv::Mat img);

  /**
   * Detect circles in a binary image.
   */
  Ball detectCircles(cv::Mat img);
};
