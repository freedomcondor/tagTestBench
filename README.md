# The Apriltag testbench
## Prerequisites
1. clone simuGL environment, which is a opengl based environment to draw objects in a 3D space. [simuGL](https://github.com/freedomcondor/simuGL.git)
2. You have to install opengl, apriltag, opencv in your computer.

## Usage
### prepare and compile
1. install opengl, opencv, apriltag in you computer
2. git clone https://github.com/freedomcondor/simuGL.git in any place, say it is ~/simuGL.
3. say you have this tagTestBench at ~/tagTestBench. Change ~/tagTestBench/CMakeList.txt line 4 to your simuGL location. In this case, ~/simuGL/src.
4. compile and run: mkdir build, cd build, cmake .., make.

## Possible Problems
1. If you have problems linking opengl libraries (especially when your are using windows), the CMakeList.txt responsible for this is located at ~/simuGL/src/GLTools/CMakeLists.txt. You may need to change it to fit your system configuration.

## How does this work
Basically, simuGL creates a opengl environment, and calls simu\_init(), simu\_step(), simu\_draw(), simu\_destroy() located in tagTestBench/MainLoop.cpp. This MainLoop.cpp is responsible for using opencv to open the camera or read a local image, using apriltag libraries to detect tags and estimate poses, and draw the result in the 3D space.
