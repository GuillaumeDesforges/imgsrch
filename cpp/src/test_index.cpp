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

int main(int argc, char** argv)
{
    string path = "../../database/";
    if(argc > 1) {
        path = argv[1];
    }

    //**** TEST : On verifie si l'adresse de la database est correcte ********
    // FILE * fichier = fopen("..\\..\\database\\", "r+");
    // if (fichier == NULL){
    //   // Le fichier n'existe pas
    //   cout<<" FATAL ERROR : The given path doesn't exist."<<endl;
    // }
    // else {
    //   // Le fichier existe et on le referme
    //   fclose(fichier);
    // }
    //*************************************************************************

    // Load images path in directory
    auto files_list = get_file_list(path);

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
    KMeansTree<Point<double>, double> kmeanstree(4, 128, 5);

    // Agregate all descriptors in all images to train KMeansTree
    vector<Point<double>> descriptors;
    for(auto& image : images) {
        for(auto& descriptor : image.getDescriptors()) {
            descriptors.push_back(descriptor);
        }
    }

    kmeanstree.addPoints(descriptors);
    kmeanstree.init();
    kmeanstree.fit();

    // ********** TEST ***********************
    // a verifier
    for(int k=0; k<10; k++){
      int r=rand()%(descriptors.size());
      Point<double> point_to_test= descriptors[r];

      vector<Point<double> > clusters=kmeanstree.getClusters();
      vector<Point<double> > means=kmeanstree.getMeans();

      int cluster_to_test=clusters(point_to_test);
      Point<double> mean_to_test=means[cluster_to_test];
      double distance_to_test=(point_to_test - mean_to_test).norm2();
      for(int p=0; p<clusters.size();p++){
        double distance=(point_to_test - means[p]).norm2();
        if (distance<distance_to_test){
          cout<< "Clusterization error !";
        }
      }
    }
    // ************************************

    // Put words to images
    for(auto& image : images) {
        image.computeWords(kmeanstree);
    }

    // Create index
    Index index;
    index.indexImages(images);

    cout << "Test : votes for " << images[0].getPath() << endl;
    map<string, double> scores = index.getScores(images[0]);
    for(auto& entry : scores) {
        cout << entry.first << " => score = " << entry.second << " ; votes = " << (entry.second * images[0].getDescriptors().size()) << endl;
    }

    return 0;
}
