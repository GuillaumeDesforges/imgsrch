cmake_minimum_required(VERSION 3.5)

project(MOPSI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB clustering_SRC
    "src/clustering/*.h"
    "src/clustering/*.cpp"
)

add_executable(clustering src/test_clustering.cpp ${clustering_SRC})

file(GLOB img_SRC
    "src/img/*.h"
    "src/img/*.cpp"
)

add_executable(img src/test_img.cpp ${clustering_SRC} ${img_SRC})
target_link_libraries(img ${OpenCV_LIBS})

add_executable(index src/test_index.cpp ${clustering_SRC} ${img_SRC})
target_link_libraries(index ${OpenCV_LIBS})
