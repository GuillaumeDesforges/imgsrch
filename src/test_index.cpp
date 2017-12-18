#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;

#include "img/image.h"
#include "clustering/kmeanstree.h"
#include "img/index.h"

#include "utils.h"

// Pour creer database = (1,{bonjour, hello, ponts}) ,  (2, {allo, bonjour}) , (3,{ponts})...}

vector<Image> load_multiple_images(vector<string> &file_list) {
    vector<Image> images;
    for(auto &img_path : file_list) {
        images.push_back(Image(img_path));
    }
    return images;
}

map<int, Image*> create_database(vector<Image> &images) {
    map<int, Image*> database;
    int k = 0;
    for(auto &image : images){
        database.insert(pair<int, Image*>(k, &image));
        k++;
    }
    return database;
}

int main(int argc, char** argv )
{
    // Load images path in directory
    auto files_list = get_file_list("../database/");

    // Load images
    vector<Image> images = load_multiple_images(files_list);

    // Create SIFT instance and compute descriptors for all images
    cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
    for(auto& image : images) {
        image.detectKeyPoints(f2d);
        image.computeDescriptors(f2d);
        // image.showKeyPoints();
    }


    // Create and train KMeansTree
    KMeansTree<double> kmeanstree(4, 128, 5);

    // Agregate all descriptors in all images to train KMeansTree
    vector<Point<double>> descriptors;
    for(auto& image : images) {
        for(auto& descriptor : image.getDescriptors()) {
            descriptors.push_back(descriptor);
        }
    }
    kmeanstree.init(descriptors);

    kmeanstree.fit();

    // Put words to images
    for(auto& image : images) {
        image.computeWords(kmeanstree);
    }

    // Create index
    Index index;
    // Create "database" to pass to index
    map<int, Image*> database = create_database(images);
    index.indexImages(database);
    // Could be image per image :
    /*
        int image_id = 0;
        for(auto& image : images) {
            index.indexImage(image_id, image);
            image_id++;
        }
    */

    cout << "Test : votes for " << files_list[0] << endl;
    map<int, int> votes = index.getVotes(images[0]);
    int n_words = images[0].getWords().size();
    map<int, double> scores;
    for(auto& image_vote : votes) {
        int id = image_vote.first;
        double vote = image_vote.second;
        double score = vote/n_words;
        scores[id] = score;
    }
    for(int i = 0; i < images.size(); i++) {
        cout << files_list[i] << " (" << images[i].getWords().size() << " words) => score = " << scores[i] << " ; votes = " << votes[i] << endl;
    }

    return 0;
}
