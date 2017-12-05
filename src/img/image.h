#pragma once

#include <string>
using namespace std;

#include <opencv2/opencv.hpp>

#include "../clustering/kmeanstree.h"

class Image {
public:
    Image(string path);
    void display();
    void detectKeyPoints(cv::Ptr<cv::Feature2D> &f2d);
    void showKeyPoints();
    void computeDescriptors(cv::Ptr<cv::Feature2D> &f2d);
    void computeWords(KMeansTree<int> &kmeanstree);
    const vector<string>& getWords() const;
private:
    string path;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    vector<string> words;
};
