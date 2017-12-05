#include "image.h"

#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

Image::Image(string path) : path(path) {
    image = cv::imread(path, 1);
    if (!image.data) {
        throw invalid_argument("path");
    }
}

void Image::display() {
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);
    cv::waitKey(0);
}

void Image::detectKeyPoints(cv::Ptr<cv::Feature2D> &f2d) {
    f2d->detect(image, keypoints);
}

void Image::showKeyPoints() {
    cv::Mat output;
    cv::drawKeypoints(image, keypoints, output);
    cv::namedWindow("Display Image Keypoints", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image Keypoints", output);
    cv::waitKey(0);
}

void Image::computeDescriptors(cv::Ptr<cv::Feature2D> &f2d) {
    f2d->compute(image, keypoints, descriptors);
}

void Image::computeWords(KMeansTree<double> &kmeanstree) {
    for(int i = 0; i < descriptors.rows; i++) {
        const double* Mi = descriptors.ptr<double>(i);
        Point<double> point(128);
        for(int j = 0; j < 128; j++) {
            point[j] = Mi[j];
        }
        words.push_back(kmeanstree.getWord(point));
    }
}

const vector<string>& Image::getWords() const {
    return words;
}
