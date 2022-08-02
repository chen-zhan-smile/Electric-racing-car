find_package(OpenCV REQUIRED)
aux_source_directory(. DIR_SRCS_QR)
#形成一个连接库 QRcode
add_library(QRcode ${DIR_SRCS_QR})
target_link_libraries(QRcode ${OpenCV_LIBS})