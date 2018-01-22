# Imgsrch

MOPSI (MOdéliser Programmer SImuler) project at ENPC Engineering School, Mathematics and CS department.

Team :
- Guillaume DESFORGES
- Maïlys Rheins
- Diane Pugès

## Goal

Identify pictures in a database that are close to a given picture.

Make a usable library.

## How

We identify features in pictures as "descriptors" using CV SIFT.

We then train a KMeans Decision Tree. This allows us to create a "picture descriptor dictionary"
listing what we could call "picture words".

Pictures that are most likely "alike" are those sharing a lot of those features.

## Install

1. Dependencies
    * libboost
        * `sudo apt-get install -y libboost-all-dev`
    * cmake (>= 3.5)
        * `sudo apt-get install -y cmake`
    * doxygen
        * `sudo apt-get install -y doxygen`
    * opencv & opencv_contrib
        * `cd ~`
        * `mkdir packages && cd packages`
        * `git clone https://github.com/opencv/opencv.git`
        * `git clone https://github.com/opencv/opencv_contrib.git`
        * `mkdir opencv_build && cd opencv_build`
        * `cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_GTK=ON -D WITH_OPENGL=ON -D OPENCV_EXTRA_MODULES_PATH=~/packages/opencv_contrib/modules ~/packages/opencv`
        * `make -j4`
        * `sudo make install`
2. Clone repository
    * `cd ~ && mkdir projects && cd projects`
    * `git clone https://github.com/GuillaumeDesforges/imgsrch`
    * `cd imgsrch`
    * `mkdir build && cd build`
3. Build
    * `cmake .. && make`
    * Build the doc : `make doc`
4. Test
    * `./clustering` should print a kmeans and kmeanstree 2D test
    * `./img img_test` should show a picture, press a key and it will show picture with SIFT keypoints, press key again to quit
    * `./index` should try to look for pictures in ../database/ and load its images, then train a kmeanstree based on the descriptors of the images, and finally compare images descriptors ressemblance with one of the image.
    * `./serialize` should train a KMeans, serialize its means, deserialize into another KMeans
