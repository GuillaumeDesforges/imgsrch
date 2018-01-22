#pragma once

#include <string>
using namespace std;

#include <opencv2/opencv.hpp>

#include "../clustering/kmeanstree.h"

class Image {
public:
    Image(string path);
    string getPath();
    void display();
    void detectKeyPoints(cv::Ptr<cv::Feature2D> &f2d);
    void showKeyPoints();
    void computeDescriptors(cv::Ptr<cv::Feature2D> &f2d);
    vector<Point<double>> getDescriptors() const;
    void computeWords(KMeansTree<Point<double>, double> &kmeanstree);
    const vector<string>& getWords() const;
private:
    string path;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    vector<Point<double>> descriptors;
    vector<string> words;
};
