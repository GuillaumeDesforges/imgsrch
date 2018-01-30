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

string Image::getPath() {
    return path;
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
    cv::Mat mat_descriptors;
    f2d->compute(image, keypoints, mat_descriptors);
    for(int i = 0; i < mat_descriptors.rows; i++) {
        Descriptor point(128);
        for(int j = 0; j < 128; j++) {
            point[j] = mat_descriptors.at<float>(i, j);
        }
        descriptors.push_back(point);
    }
}

vector<cv::KeyPoint> Image::getKeyPoints() const {
    return keypoints;
}

vector<Descriptor> Image::getDescriptors() const {
    return descriptors;
}

void Image::computeWords(KMeansTree<Descriptor, float> &kmeanstree) {
    int i = 0;
    for(auto& point : descriptors) {
        words.push_back(kmeanstree.getWord(point));
        // cout << "\r" << i++ << flush;
    }
    // cout << endl;
}

const vector<string>& Image::getWords() const {
    return words;
}
