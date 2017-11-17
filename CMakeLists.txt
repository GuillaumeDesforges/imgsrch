project(MOPSI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB clustering_SRC
    "clustering/*.h"
    "clustering/*.cpp"
)

add_executable(clustering ${clustering_SRC})

file(GLOB img_SRC
    "img/*.h"
    "img/*.cpp"
)

add_executable(img ${img_SRC})
target_link_libraries(img ${OpenCV_LIBS})
