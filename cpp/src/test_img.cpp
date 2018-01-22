#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "img/image.h"
#include "clustering/kmeanstree.h"

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: img <image path>\n");
        return -1;
    }

    Image img(argv[1]);
    img.display();

    cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
    img.detectKeyPoints(f2d);
    img.computeDescriptors(f2d);
    img.showKeyPoints();

    KMeansTree<Point<double>, double> kmeanstree(4, 128, 2, "");
    vector<Point<double>> descriptors = img.getDescriptors();
    kmeanstree.addPoints(descriptors);
    kmeanstree.init();
    kmeanstree.fit();

    cout << kmeanstree.getWord(descriptors[0]) << endl;

    return 0;
}
