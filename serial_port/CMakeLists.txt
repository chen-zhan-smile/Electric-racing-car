aux_source_directory(. DIR_SRCS_SERIAL)
include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(serial ${DIR_SRCS_SERIAL})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)