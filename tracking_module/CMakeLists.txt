
find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)
#message(STATUS "OpenCV version: "${OpenCV_VERSION})
#message(STATUS "include path:"${OpenCV_INCLUDE_DIRS})
aux_source_directory(. DIR_SRCS_Tracking_Module)
#形成一个连接库 Tracking_Module
add_library(Tracking_Module ${DIR_SRCS_Tracking_Module})
target_link_libraries(Tracking_Module ${OpenCV_LIBS})
target_link_libraries(Tracking_Module fmt)