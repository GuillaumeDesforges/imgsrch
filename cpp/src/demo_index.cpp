#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;

#include "img/image.h"
#include "clustering/kmeanstree.h"
#include "img/index.h"

#include "utils.h"

typedef Point<double> Descriptor;

vector<Image> load_multiple_images(vector<string> &file_list) {
    vector<Image> images;
    int counter = 0;
    for(auto &img_path : file_list) {
        images.push_back(Image(img_path));
        cout << "\rLoaded " << counter++ << " images " << endl;
    }
    return images;
}

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


int main(int argc, char** argv)
{
    string path = "../../database/";
    if(argc > 1) {
        path = argv[1];
    }
    // Load images path in directory
    auto files_list = get_file_list(path);

    // Load images
    vector<Image> images = load_multiple_images(files_list);

    int counter;
    
    // Create SIFT instance and compute descriptors for all images
    cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
    counter = 1;
    for(auto& image : images) {
        image.detectKeyPoints(f2d);
        cout << image.getKeyPoints().size() << " descriptors to compute." << endl;
        image.computeDescriptors(f2d);
        // image.showKeyPoints();
        cout << "Computed " << counter++ << " images' descriptors." << endl;
    }

    // Agregate part of the descriptors from all images to train KMeansTree
    vector<Descriptor> descriptors;
    for(auto& image : images) {
        vector<Descriptor> imageDescriptors = image.getDescriptors();
        for(int i = 0; i < imageDescriptors.size(); i++) {
            Descriptor descriptor = imageDescriptors[i];
            descriptors.push_back(descriptor);
        }
    }
    // Select only a part of all descriptors
    const int descriptorSelectionSize = 20000;
    cout << "Selecting " << descriptorSelectionSize << " descriptors" << endl;
    random_unique(descriptors.begin(), descriptors.end(), descriptorSelectionSize);
    vector<Descriptor> trainingSetDescriptors;
    for(int i = 0; i < descriptorSelectionSize; i++) {
        trainingSetDescriptors.push_back(descriptors[i]);
    }

    // Create and train KMeansTree
    const int kmeanstreeDepth = 6;
    const int kmeanstreeNumberOfClusters = 5;
    KMeansTree<Descriptor, double> kmeanstree(kmeanstreeNumberOfClusters, 128, kmeanstreeDepth);
    cout << "Instianciating KMeansTree" << endl;
    
    kmeanstree.addPoints(trainingSetDescriptors);
    cout << trainingSetDescriptors.size() << " descriptors added for training" << endl;
    kmeanstree.init();
    cout << "Training KMeansTree" << endl;
    kmeanstree.fit();

    // Put words to images
    counter = 1;
    cout << "Computing images' words list" << endl;
    for(auto& image : images) {
        image.computeWords(kmeanstree);
        cout << "Computed " << counter++ << " image words" << endl;
    }

    // Create index
    cout << "Instanciating index" << endl;
    Index index;
    index.indexImages(images);

    // Count word repartition
    for(auto& word : index.getPossibleWords()) {
        cout << word << " : " << index.getImagesPathWithWord(word).size() << endl;
    }
    cout << "Dictionnary has " << index.getPossibleWords().size() << " words" << endl;

    // Search
    cout << "Test : votes for " << images[0].getPath() << endl;
    map<string, double> scores = index.getScores(images[0]);
    for(auto& entry : scores) {
        cout << entry.first << " => score = " << entry.second << endl;
    }

    return 0;
}
