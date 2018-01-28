cmake_minimum_required(VERSION 3.5)

project(MOPSI)

set(CMAKE_CXX_STANDARD 14)

# ============= Executable

find_package(Boost REQUIRED system filesystem serialization)
find_package(OpenCV)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories("${Boost_LIBRARY_DIRS}")

file(GLOB clustering_SRC
    "src/clustering/*.h"
    "src/clustering/*.cpp"
)

add_executable(clustering src/demo_clustering.cpp ${clustering_SRC})
target_link_libraries(clustering LINK_PUBLIC ${Boost_LIBRARIES})

add_executable(serialize src/demo_serialize.cpp ${clustering_SRC})
target_link_libraries(serialize LINK_PUBLIC ${Boost_LIBRARIES})

file(GLOB img_SRC
    "src/img/*.h"
    "src/img/*.cpp"
)

add_executable(img src/demo_img.cpp ${clustering_SRC} ${img_SRC})
target_link_libraries(img LINK_PUBLIC ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_executable(index src/demo_index.cpp ${clustering_SRC} ${img_SRC})
target_link_libraries(index LINK_PUBLIC ${OpenCV_LIBS} ${Boost_LIBRARIES})

# ============= Documentation

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

# ============= Unit test

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
    CXXTEST_ADD_TEST(test_point test_point.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/clustering/point.unittest.h)
endif()
