cmake_minimum_required(VERSION 3.12)

project(main)

#设置线程选项
set(CMAKE_CXX_FLAGS -pthread)
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)
message(STATUS "OpenCV version: "${OpenCV_VERSION})
message(STATUS "include path:"${OpenCV_INCLUDE_DIRS})
#添加两个子目录 串口通信模块 二维码识别模块
aux_source_directory(. DIR_SRCS)
add_subdirectory(serial_port)
add_subdirectory(QRcode_detector)
add_subdirectory(detector_numbers)
add_subdirectory(tracking_module)

 
add_executable(main ${DIR_SRCS})
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(main fmt)
target_link_libraries(main QRcode)
target_link_libraries(main serial)
target_link_libraries(main Detector_Num)
target_link_libraries(main Tracking_Module)

