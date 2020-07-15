# EigenFlow
This project was inspired by Deep Learning From Scratch: Theory and Implementation [blog post](https://www.deepideas.net/deep-learning-from-scratch-theory-and-implementation/) by Daniel Sabinasz. This project implements an API to create computational graphs and neural nets using Eigen libraray in C++ from scratch. Corrently it includes a gradient descent optimizer to minimize any user defined loss function. This project was created with educational purposes in mind and does not offer the most efficient way of implementing neural nets. It corrently uses dynamicly sized matrices everywhere. However, for large matrices the overhead might be reasonable and still efficient according to Eigen library documentation.

## Dependencies for Running 
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1 - Not corrently used
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* Eigen3 >= 3.3
    * All Oses: [download and extract Eigen's source code](http://eigen.tuxfamily.org/index.php?title=Main_Page#Download). 
    * The directory in which you placed Eigen's source code must be in the include path
    * Or use the -I option with GCC to achieve:
     ``` g++ -I /path/to/eigen/ my_program.cpp -o my_program ```

## Basic Build Instructions
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./nn`.

