cmake_minimum_required(VERSION 3.0.2)
project(image_publisher)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  std_msgs
  camera_info_manager
)

find_package(OpenCV)

catkin_package(
  # CATKIN_DEPENDS roscpp cv_bridge image_transport sensor_msgs std_msgs
  # DEPENDS opencv
)

include_directories(
  calibration
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} src/image_publisher.cpp)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
