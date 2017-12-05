#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "img/image.h"

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Image img(argv[1]);
    img.display();

    cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
    img.detectKeyPoints(f2d);
    img.computeDescriptors(f2d);
    img.showKeyPoints();

    return 0;
}
