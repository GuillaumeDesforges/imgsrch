#pragma once

#include<Python.h>

#include <string>
#include <map>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "../cpp/src/utils.h"

#include "../cpp/src/clustering/point.h"
#include "../cpp/src/clustering/kmeanstree.h"
#include "../cpp/src/img/image.h"
#include "../cpp/src/img/index.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

typedef Point<double> Descriptor;

const int kmeansClusterNumber = 5;
const int kmeanstreeDepth = 4;
const int kmeanstreeTrainingSampleSize = 50000;

template<class bidiiter>
bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        bidiiter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

class ImgSrch {
public:
    ImgSrch() {
        kmeanstree = new KMeansTree<Descriptor, double>(kmeansClusterNumber, 128, kmeanstreeDepth);
        index = new Index();
    }

    ~ImgSrch() {
        delete kmeanstree;
        delete index;
    }

    int train(const char* path) {
        try {
            cout << endl;
            // Load file list
            // cout << "Loading file list" << endl;
            auto files_list = get_file_list(path);
            
            // Create SIFT instance
            // cout << "Creating SIFT instance" << endl;
            cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();

            // Creating data for training
            cout << "Creating training dataset for KMeansTree" << endl;
            int sampleSizePerImage = kmeanstreeTrainingSampleSize / files_list.size();
            vector<Descriptor> trainingDescriptors;
            for(auto &img_path : files_list) {
                cout << "Image : " << img_path << endl;
                Image image(img_path);
                // cout << "Computing image descriptors" << endl;
                image.detectKeyPoints(f2d);
                image.computeDescriptors(f2d);
                vector<Descriptor> imageDescriptors = image.getDescriptors();
                random_unique(imageDescriptors.begin(), imageDescriptors.end(), sampleSizePerImage);
                // cout << "Sampling" << endl;
                for(int i = 0; i < sampleSizePerImage; i++) {
                    Descriptor descriptor = imageDescriptors[i];
                    trainingDescriptors.push_back(descriptor);
                }
            }

            // Init and train KMeansTree
            // cout << "Adding training points" << endl;
            kmeanstree->addPoints(trainingDescriptors);
            kmeanstree->init();
            cout << "Training" << endl;
            kmeanstree->fit();
        } catch(const std::exception& e) {
            return 1;
        }
        return 0;
    };

    int indexDirectory(const char* path) {
        try {
            cout << endl;
            // Load file list
            cout << "Loading file list" << endl;
            auto files_list = get_file_list(path);
            
            // Create SIFT instance
            cout << "Creating SIFT instance" << endl;
            cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
            
            // Put words to images and index them
            for(auto &img_path : files_list) {
                cout << "Image : " << img_path << endl;
                Image image(img_path);
                // cout << "Computing image descriptors" << endl;
                image.detectKeyPoints(f2d);
                image.computeDescriptors(f2d);
                // cout << "Computing image words" << endl;
                image.computeWords(*kmeanstree);
                // cout << "Indexing" << endl;
                (*index).indexImage(image);
            }
        } catch(const std::exception& e) {
            return 1;
        }
        return 0;
    }

    map<string, double> computeLikelihoods(const char* path) {
        string p = string(path);
        Image inputImg(p);
        cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
        inputImg.detectKeyPoints(f2d);
        inputImg.computeDescriptors(f2d);
        inputImg.computeWords(*kmeanstree);

        map<string, double> scores = index->getScores(inputImg);
        return scores;
    }

    void write(string file) {
        std::ofstream ofs(file.c_str());
        boost::archive::text_oarchive oa(ofs);
        oa << (*kmeanstree);
    }

    void read(string file) {
        std::ifstream ifs(file.c_str());
        boost::archive::text_iarchive ia(ifs);
        ia >> (*kmeanstree);
    }

    KMeansTree<Descriptor, double> *kmeanstree;
    Index *index;
};
