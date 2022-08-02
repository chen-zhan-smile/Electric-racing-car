
find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)
#message(STATUS "OpenCV version: "${OpenCV_VERSION})
#message(STATUS "include path:"${OpenCV_INCLUDE_DIRS})
aux_source_directory(. DIR_SRCS_Detector_Num)
#形成一个连接库 QRcode
add_library(Detector_Num ${DIR_SRCS_Detector_Num})
target_link_libraries(Detector_Num ${OpenCV_LIBS})
target_link_libraries(Detector_Num fmt)


