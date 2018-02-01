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
        * check you have a version recent enough with `cmake --version`
    * doxygen
        * `sudo apt-get install -y doxygen`
    * cxxtest
        * `sudo apt-get install -y cxxtest`
    * opencv & opencv_contrib
        * `cd ~`
        * `mkdir packages && cd packages`
        * `git clone https://github.com/opencv/opencv.git`
        * `git clone https://github.com/opencv/opencv_contrib.git`
        * `mkdir opencv_build && cd opencv_build`
        * `cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_GTK=ON -D WITH_OPENGL=ON -D OPENCV_EXTRA_MODULES_PATH=~/packages/opencv_contrib/modules ~/packages/opencv`
        * Note: you might need to add GPU support, read OpenCV doc for more info
        * `make -j4` where 4 is the number of threads you can have on your computer
        * `sudo make install`
2. Clone repository
    * `cd ~ && mkdir projects && cd projects`
    * `git clone https://github.com/GuillaumeDesforges/imgsrch`
    * `cd imgsrch`
3. Build
    * `./autobuild.sh`
    * In _cpp_ folder `cmake .. && make` is the old way
    * Build the doc : `make doc`
4. Test
    * Unit tests can be run with command `make test`

## Demo
    * `./demo_clustering` prints a kmeans and kmeanstree 2D test
    * `./demo_img img_test` shows a picture, press a key and it will show picture with SIFT keypoints, press key again to quit
    * `./demo_index` tries to look for pictures in ../database/ and load its images, then train a kmeanstree based on the descriptors of the images, and finally compare images descriptors ressemblance with one of the image.
    * `./demo_serialize` trains a KMeans, serialize its means, deserialize into another KMeans

## Python
Once compiled, both `_imgsrch.so` and `imgsrch.py` are generated. Copy/paste them in the folder of your project to use them.

You can train your own engine :
```
from imgsrch import Engine
engine = Engine(4, 6) # 4 clusters, depth of 6
trained = engine.train("path/to/images/folder/", 1000000) # train KMeansTree on a random sample of total size 1000000
if not trained:
    print("Could not train KMeansTree")
    exit(1)
indexed = engine.indexImages("path/to/images/folder/")
if not indexed:
    print("Could not index directory")
    exit(1)
engine.write("engine.xml")
```

And then in your application:
```
from imgsrch import Engine
engine = Engine("engine.xml")
```

## Contribute
There is much to do !

* make C++ call Python exceptions instead of return bool
* add documentation
* make the training more efficient
* add score to images with very close words ("aab" should add a bit of score to images with "aa")
