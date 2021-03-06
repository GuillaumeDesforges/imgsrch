#pragma once

#include<Python.h>

#include <string>
#include <map>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "utils.h"

#include "clustering/point.h"
#include "clustering/kmeanstree.h"
#include "img/image.h"
#include "img/index.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

typedef Point<float> Descriptor;

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

class Engine{
    public:
        Engine(string file) {
            try {
                // Input archive
                std::ifstream ifs(file.c_str());
                boost::archive::xml_iarchive ia(ifs);
                // Read
                ia >> BOOST_SERIALIZATION_NVP(kmeanstree);
                ia >> BOOST_SERIALIZATION_NVP(index);
            } catch(const std::exception& e) {
                cout << "Failed to read from file, an exception has occured." << endl;
                cerr << e.what() << endl;
            }
        }
        
        Engine(int kmeansClusterNumber, int kmeanstreeDepth) 
        : kmeanstree(kmeansClusterNumber, 128, kmeanstreeDepth) {
        }

        bool train(const char* path, int kmeanstreeTrainingSampleSize) {
            try {
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
                    cout << "Image : " << img_path;
                    Image image(img_path);
                    // cout << "Computing image descriptors" << endl;
                    image.detectKeyPoints(f2d);
                    image.computeDescriptors(f2d);
                    vector<Descriptor> imageDescriptors = image.getDescriptors();
                    int sampleSizeForThisImage = (sampleSizePerImage < image.getDescriptors().size()) ? sampleSizePerImage : image.getDescriptors().size();
                    random_unique(imageDescriptors.begin(), imageDescriptors.end(), sampleSizeForThisImage);
                    // cout << "Sampling" << endl;
                    for(int i = 0; i < sampleSizeForThisImage; i++) {
                        Descriptor descriptor = imageDescriptors[i];
                        trainingDescriptors.push_back(descriptor);
                    }
                    cout << ", " << sampleSizeForThisImage << endl;
                }

                // Init and train KMeansTree
                // cout << "Adding training points" << endl;
                kmeanstree.addPoints(trainingDescriptors);
                kmeanstree.init();
                cout << "Training" << endl;
                kmeanstree.fit();
            } catch(const std::exception& e) {
                cout << "Failed to train engine, an exception has occured." << endl;
                cerr << e.what() << endl;
                return false;
            }
            return true;
        };

        bool indexDirectory(const char* path) {
            try {
                // Load file list
                cout << "Loading file list" << endl;
                auto files_list = get_file_list(path);

                // Create SIFT instance
                cout << "Creating SIFT instance" << endl;
                cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();

                // Put words to images and index them
                for(auto &img_path : files_list) {
                    cout << "Image : " << img_path;
                    Image image(img_path);
                    image.detectKeyPoints(f2d);
                    image.computeDescriptors(f2d);
                    cout << " " << image.getDescriptors().size() << endl;
                    image.computeWords(kmeanstree);
                    index.indexImage(image);
                }
            } catch(const std::exception& e) {
                cout << "Failed to index directory, an exception has occured." << endl;
                cerr << e.what() << endl;
                return false;
            }
            return true;
        }

        map<string, float> computeLikelihoods(const char* path) {
            map<string, float> scores;
            try {
                string p = string(path);
                Image inputImg(p);
                cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
                inputImg.detectKeyPoints(f2d);
                inputImg.computeDescriptors(f2d);
                inputImg.computeWords(kmeanstree);
                scores = index.getScores(inputImg);
            } catch(const std::exception& e) {
                cout << "Failed to compute likelihood, an exception has occured." << endl;
                cerr << e.what() << endl;
                return scores;
            }
            return scores;
        }

        bool write(string file) {
            try {
                std::ofstream ofs(file.c_str());
                boost::archive::xml_oarchive oa(ofs);
                oa << BOOST_SERIALIZATION_NVP(kmeanstree);
                oa << BOOST_SERIALIZATION_NVP(index);
            } catch(const std::exception& e) {
                cout << "Failed to write to file, an exception has occured." << endl;
                cerr << e.what() << endl;
                return false;
            }
            return true;
        }
    private:
        KMeansTree<Descriptor, float> kmeanstree;
        Index index;
};
