#include "image.h"

#include <iostream>
#include <stdexcept>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
using namespace cv;

Image::Image(string path) : path(path) {
    image = imread(path, 1);
    if (!image.data) {
        throw invalid_argument("path");
    }
}

void Image::display() {
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    waitKey(0);
}

void Image::detectKeyPoints(Ptr<Feature2D> &f2d) {
    f2d->detect(image, keypoints);
}

void Image::showKeyPoints() {
    Mat output;
    drawKeypoints(image, keypoints, output);
    namedWindow("Display Image Keypoints", WINDOW_AUTOSIZE);
    imshow("Display Image Keypoints", output);
    waitKey(0);
}

void Image::computeDescriptors(Ptr<Feature2D> &f2d) {
    f2d->compute(image, keypoints, descriptors);
}

void Image::computeWords(KMeansTree<int> &kmeanstree) {
    // TODO implement descriptors to words computation through trained kmeans
}

const vector<string>& Image::getWords() const {
    return words;
}
