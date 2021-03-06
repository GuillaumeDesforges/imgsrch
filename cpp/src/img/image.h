#pragma once

#include <string>
using namespace std;

#include <opencv2/opencv.hpp>

#include "../clustering/kmeanstree.h"

typedef Point<float> Descriptor;

/**
    \class Image
    \brief Usual representation of a point in space T^d
**/
class Image {
public:
    Image(string path);
    string getPath();
    void display();
    void detectKeyPoints(cv::Ptr<cv::Feature2D> &f2d);
    void showKeyPoints();
    void computeDescriptors(cv::Ptr<cv::Feature2D> &f2d);
    vector<cv::KeyPoint> getKeyPoints() const;
    vector<Descriptor> getDescriptors() const;
    void computeWords(KMeansTree<Descriptor, float> &kmeanstree);
    const vector<string>& getWords() const;
private:
    string path;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    vector<Descriptor> descriptors;
    vector<string> words;
};
