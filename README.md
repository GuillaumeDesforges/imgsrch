# Imgsrch

MOPSI (MOdéliser Programmer SImuler) project at ENPC Engineering School, Mathematics and CS department.

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
    * cmake
* Clone repository
* Build
    * Go to project folder
    * Create _build_ folder
    * Go to that _build_ folder
    * On Linux : `cmake .. && make`
* Test
    * On Linux : `./bin/clustering` should print a kmeans 1D test
