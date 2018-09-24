# DNT-Introduction
Introduction for the Dutch Nao Team: learn how to program robots to play football.

Requirements:

* [Naoqi](http://doc.aldebaran.com/index.html)
  - Go to https://community.ald.softbankrobotics.com/en/resources/software/language/en-gb and ask one of the current team members for the login credentials. </br>
If you are using a V5 robot, download for your OS: </br>
`[FORMER NAO VERSIONS] - Python NAOqi SDK` for Python. </br>
`[FORMER NAO VERSIONS] - C++ NAOqi SDK` for C++. </br>
When using a V6 download for your OS: </br>
`[NAO V6 ONLY] - SDKs and documentation 2.8.3 View archives`
  - For Python check http://doc.aldebaran.com/2-1/dev/python/install_guide.html for how to install
  - **NOTE**: If you get linking errors, please ask one of our team members for a working SDK.
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
