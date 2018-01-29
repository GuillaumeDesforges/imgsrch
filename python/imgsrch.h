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

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

typedef Point<double> Descriptor;

const int kmeansClusterNumberDefault = 5;
const int kmeanstreeDepthDefault = 4;
const int kmeanstreeTrainingSampleSizeDefault = 50000;

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
        Engine(int kmeansClusterNumber = kmeansClusterNumberDefault, int kmeanstreeDepth = kmeanstreeDepthDefault) 
        : kmeanstree(kmeansClusterNumber, 128, kmeanstreeDepth) {

        }

        bool train(const char* path, int kmeanstreeTrainingSampleSize = kmeanstreeTrainingSampleSizeDefault) {
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
                    cout << "Image : " << img_path << endl;
                    Image image(img_path);
                    // cout << "Computing image descriptors" << endl;
                    image.detectKeyPoints(f2d);
                    image.computeDescriptors(f2d);
                    // cout << "Computing image words" << endl;
                    image.computeWords(kmeanstree);
                    // cout << "Indexing" << endl;
                    index.indexImage(image);
                }
            } catch(const std::exception& e) {
                cout << "Failed to index directory, an exception has occured." << endl;
                cerr << e.what() << endl;
                return false;
            }
            return true;
        }

        map<string, double> computeLikelihoods(const char* path) {
            map<string, double> scores;
            try {
                cout << "a" << endl;
                string p = string(path);
                Image inputImg(p);
                cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
                cout << "a" << endl;
                inputImg.detectKeyPoints(f2d);
                inputImg.computeDescriptors(f2d);
                cout << "a" << endl;
                inputImg.computeWords(kmeanstree);
                cout << "a" << endl;
                scores = index.getScores(inputImg);
                cout << "a" << endl;
            } catch(const std::exception& e) {
                cout << "Failed to compute likelyhood, an exception has occured." << endl;
                cerr << e.what() << endl;
                return scores;
            }
            return scores;
        }

        bool write(string file) {
            try {
                std::ofstream ofs(file.c_str());
                boost::archive::xml_oarchive oa(ofs);
                Engine engine = *this;
                oa << BOOST_SERIALIZATION_NVP(engine);
            } catch(const std::exception& e) {
                cout << "Failed to write to file, an exception has occured." << endl;
                cerr << e.what() << endl;
                return false;
            }
            return true;
        }

        bool read(string file) {
            try {
                // Input archive
                std::ifstream ifs(file.c_str());
                boost::archive::xml_iarchive ia(ifs);
                // Read
                Engine &engine = *this;
                ia >> BOOST_SERIALIZATION_NVP(engine);
            } catch(const std::exception& e) {
                cout << "Failed to read from file, an exception has occured." << endl;
                cerr << e.what() << endl;
                return false;
            }
            return true;
        }
    private:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version) {
                ar & boost::serialization::make_nvp("kmeanstree", kmeanstree);
                ar & boost::serialization::make_nvp("index", index);
            }
        KMeansTree<Descriptor, double> kmeanstree;
        Index index;
};
