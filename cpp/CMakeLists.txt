cmake_minimum_required(VERSION 3.5)

project(MOPSI)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.40 COMPONENTS system filesystem REQUIRED)
find_package(OpenCV)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB clustering_SRC
    "src/clustering/*.h"
    "src/clustering/*.cpp"
)

add_executable(clustering src/test_clustering.cpp ${clustering_SRC})
add_executable(serialize src/test_serialize.cpp ${clustering_SRC})

file(GLOB img_SRC
    "src/img/*.h"
    "src/img/*.cpp"
)

add_executable(img src/test_img.cpp ${clustering_SRC} ${img_SRC})
target_link_libraries(img ${OpenCV_LIBS})

add_executable(index src/test_index.cpp ${clustering_SRC} ${img_SRC})
target_link_libraries(index LINK_PUBLIC ${OpenCV_LIBS} ${Boost_LIBRARIES})

find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
   message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif()

set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${doxyfile_in} ${doxyfile} @ONLY)

add_custom_target(doc
   COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Generating API documentation with Doxygen"
   VERBATIM)
