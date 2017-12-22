#pragma once

#include <string>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "../src/utils.h"

#include "../src/clustering/point.h"
#include "../src/clustering/kmeanstree.h"
#include "../src/img/image.h"
#include "../src/img/index.h"


class ImgSrch {
public:
    ImgSrch() {
        kmeanstree = new KMeansTree<double>(4, 128, 2);
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
            map<int, Image*> database;
            int k = 0;
            for(auto &image : *images){
                database.insert(pair<int, Image*>(k, &image));
                k++;
            }
            Index &idx = *index;
            idx.indexImages(database);
        } catch(const std::exception& e) {
            return 1;
        }
        return 0;
    };

    void votes(const char* path) {
        string p = string(path);
        Image inputImg(p);
        cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
        inputImg.detectKeyPoints(f2d);
        inputImg.computeDescriptors(f2d);
        inputImg.computeWords(*kmeanstree);

        map<int, int> votes = index->getVotes(inputImg);
        int n_words = inputImg.getWords().size();
        map<int, double> scores;
        for(auto& image_vote : votes) {
            int id = image_vote.first;
            double vote = image_vote.second;
            double score = vote/n_words;
            scores[id] = score;
        }
        for(int i = 0; i < images->size(); i++) {
            cout << (*images)[i].getPath() << " (" << (*images)[i].getWords().size() << " words) => score = " << scores[i] << " ; votes = " << votes[i] << endl;
        }
    }

    KMeansTree<double> *kmeanstree;
    vector<Image> *images;
    Index *index;
};
