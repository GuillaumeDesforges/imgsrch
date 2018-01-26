#pragma once

#include<Python.h>

#include <string>
#include <map>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "../cpp/src/utils.h"

#include "../cpp/src/clustering/point.h"
#include "../cpp/src/clustering/kmeanstree.h"
#include "../cpp/src/img/image.h"
#include "../cpp/src/img/index.h"


class ImgSrch {
public:
    ImgSrch() {
        kmeanstree = new KMeansTree<Point<double>, double>(4, 128, 2);
        images = new std::vector<Image>();
        index = new Index();
    }
    ~ImgSrch() {
        delete kmeanstree;
        delete images;
        delete index;
    }
    int load(const char* path) {
        try {
            // Load files
            auto files_list = get_file_list(path);
            for(auto &img_path : files_list) {
                images->push_back(Image(img_path));
            }

            // Create SIFT instance and compute descriptors for all images
            cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
            for(auto& image : *images) {
             image.detectKeyPoints(f2d);
             image.computeDescriptors(f2d);
            }

            // Agregate all descriptors in all images to train KMeansTree
            vector<Point<double>> descriptors;
            for(auto& image : *images) {
                for(auto& descriptor : image.getDescriptors()) {
                    descriptors.push_back(descriptor);
                }
            }

            // Init and train KMeansTree
            kmeanstree->addPoints(descriptors);
            kmeanstree->init();
            kmeanstree->fit();

            // Put words to images
            for(auto& image : *images) {
                image.computeWords(*kmeanstree);
            }

            // Create "database" to pass to index
            Index &idx = *index;
            idx.indexImages(*images);
        } catch(const std::exception& e) {
            return 1;
        }
        return 0;
    };

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

    KMeansTree<Point<double>, double> *kmeanstree;
    vector<Image> *images;
    Index *index;
};
