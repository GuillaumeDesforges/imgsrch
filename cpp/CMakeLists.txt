# PROJECT

cmake_minimum_required(VERSION 3.5)

project(MOPSI)

set(CMAKE_CXX_STANDARD 14)

# ============= Dependencies

# boost
find_package(Boost REQUIRED system filesystem serialization)
if(NOT Boost_FOUND)
   message(FATAL_ERROR "Boost is needed to build.")
endif()
include_directories(${Boost_INCLUDE_DIRS})
link_directories("${Boost_LIBRARY_DIRS}")
# opencv
find_package(OpenCV)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV 3 is needed to build.")
endif()
include_directories(${OpenCV_INCLUDE_DIRS})
# swig
FIND_PACKAGE(SWIG REQUIRED)
if(NOT SWIG_FOUND)
    message(FATAL_ERROR "Swig is needed to build.")
endif()
INCLUDE(${SWIG_USE_FILE})
FIND_PACKAGE(PythonLibs 3 REQUIRED)
if(NOT PythonLibs_FOUND)
    message(FATAL_ERROR "PythonLibs are needed to build.")
endif()
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
# doxygen
find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
   message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif()
# cxxtest
find_package(CxxTest)
if(NOT CXXTEST_FOUND)
    message(FATAL_ERROR "CxxTest is needed to build the unit tests.")
endif()
include_directories(${CXXTEST_INCLUDE_DIR})

# ============= Demos

file(GLOB clustering_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clustering/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clustering/*.cpp"
)

file(GLOB img_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/img/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/img/*.cpp"
)

add_executable(clustering ${CMAKE_CURRENT_SOURCE_DIR}/src/demo_clustering.cpp ${clustering_SRC})
target_link_libraries(clustering LINK_PUBLIC ${Boost_LIBRARIES})

add_executable(serialize ${CMAKE_CURRENT_SOURCE_DIR}/src/demo_serialize.cpp ${clustering_SRC})
target_link_libraries(serialize LINK_PUBLIC ${Boost_LIBRARIES})

add_executable(img ${CMAKE_CURRENT_SOURCE_DIR}/src/demo_img.cpp ${clustering_SRC} ${img_SRC})
target_link_libraries(img LINK_PUBLIC ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_executable(index ${CMAKE_CURRENT_SOURCE_DIR}/src/demo_index.cpp ${clustering_SRC} ${img_SRC})
target_link_libraries(index LINK_PUBLIC ${OpenCV_LIBS} ${Boost_LIBRARIES})

# ============= Documentation

set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(doxyfile gen/Doxyfile)

configure_file(${doxyfile_in} ${doxyfile} @ONLY)

add_custom_target(doc
   COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Generating API documentation with Doxygen"
   VERBATIM)

# ============= Unit test

enable_testing()
CXXTEST_ADD_TEST(test_point gen/test_point.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/clustering/point.unittest.h)
CXXTEST_ADD_TEST(test_kmeans gen/test_kmeans.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/clustering/kmeans.unittest.h)
CXXTEST_ADD_TEST(test_kmeanstree gen/test_kmeanstree.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/clustering/kmeanstree.unittest.h)

# ============= Python

SET(CMAKE_SWIG_FLAGS "")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)

SET_SOURCE_FILES_PROPERTIES(src/imgsrch.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_LIBRARY(imgsrch LANGUAGE python SOURCES src/imgsrch.i src/engine.h ${clustering_SRC} ${img_SRC} src/utils.h)
SWIG_LINK_LIBRARIES(imgsrch ${PYTHON_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})
