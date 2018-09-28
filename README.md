# DNT-Introduction
Introduction for the Dutch Nao Team: learn how to program robots to play football. If you have any problems with installing or the code, please email me (caitlin@dutchnaoteam.nl).
The `complete_code` folder contains the complete code to let a robot play football. In `template_code` the functions exists, but are not filled. In `DNT_Introduction.pdf` a introduction to our team and how to use this code is given.

After installing all requirements, please check if everything is working correctly by compiling the code in `complete_code`. If this doesn't work, checkout trouble shooting below, or ask me for help.


Requirements:

* [Naoqi](http://doc.aldebaran.com/index.html)
  - Go to https://community.ald.softbankrobotics.com/en/resources/software/language/en-gb and ask one of the current team members for the login credentials. </br>
If you are using a V5 robot, download for your OS: </br>
`[FORMER NAO VERSIONS] - Python NAOqi SDK` for Python. </br>
`[FORMER NAO VERSIONS] - C++ NAOqi SDK` for C++. </br>
When using a V6 download for your OS: </br>
`[NAO V6 ONLY] - SDKs and documentation 2.8.3`
  - For Python check http://doc.aldebaran.com/2-1/dev/python/install_guide.html for how to install
  - **NOTE**: If you get linking errors, please ask one of our team members for a working SDK or email me.
  - For C++ check http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html
    - **NOTE**: We want to use our own OpenCV and not the one from Naoqi, so after installing OpenCV (next step) http://doc.aldebaran.com/2-4/dev/cpp/examples/vision/opencv.html:
      - Once you have OpenCV installed for your system, you have to remove it from the SDK. Do not remove the CMake configuration files.
      - Remove all OpenCV libraries from the lib/ directory of the SDK. They will have the following format: libopencv_modulename.so, libcv.so, libhighui.so and libml.so
      - Remove the opencv folder from the include folder

* [OpenCV](https://opencv.org/)
  - Please google yourself how to install it for your OS.

* [NumPy](http://www.numpy.org/)
  - `pip install numpy`.


Compile cpp code with naoqi:
  * In the same folder as CMakeLists.txt
    - A file `qiproject.xml` should exist
    - `qibuild init`
    - `qibuild add-config mytoolchain -t mytoolchain --default`, where `mytoolchain` is the name of the toolchain you created at installing the sdk for cpp
    - `qibuild configure`
    - `qibuild make`



<h2> Trouble shooting </h2>

* If you get an error with `cv::HOUGH_LINES` change the lines 65 and 99 in `orangeball.cpp` to the line that are commented above it.
* If you get `undefined reference` to something related to Naoqi (for example
  `undefined reference to qi::log::log`) use `g++-4.8`.
* If you get `undefined reference` to something related to OpenCV (for example
  `undefined reference to cv::imread`) you probably have some linking problems
  with opencv. Please try to compile the `opencv_test/` code to see if you have
  the same problem with that code. (https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_gcc_cmake/linux_gcc_cmake.html) </br>
    In `opencv_test`:
    - `mkdir build`
    - `cd build/`
    - `cmake ..`
    - `make`
    - `./DisplayImage ../../../../img/1.jpg`

  If you have problems using that code, your opencv is not installed or linked
  correctly, otherwise the combination with Naoqi and OpenCV is not working correctly.
* If you get an error about the fallmanager when you try the kick, try: http://doc.aldebaran.com/2-1/naoqi/motion/reflexes-fall-manager.html
