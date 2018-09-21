#include <limits>

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
   * Detect an orange ball in the given image.
   */
  Ball detectOrangeBall(cv::Mat img);

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
