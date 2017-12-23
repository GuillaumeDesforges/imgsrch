cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs 3 REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

find_package(Boost 1.40 COMPONENTS system filesystem REQUIRED)
find_package(OpenCV)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(imgsrch.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(imgsrch python imgsrch.i ../cpp/src/img/image.cpp ../cpp/src/img/index.cpp)
SWIG_LINK_LIBRARIES(imgsrch ${PYTHON_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})
