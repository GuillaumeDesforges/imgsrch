#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);

    Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
    std::vector<cv::KeyPoint> keypoints;
    Mat descriptors;
    f2d->detect(image, keypoints);
    f2d->compute(image, keypoints, descriptors );

    // Add results to image and save.
    Mat output;
    drawKeypoints(image, keypoints, output);

    namedWindow("Display Image Keypoints", WINDOW_AUTOSIZE );
    imshow("Display Image Keypoints", output);
    waitKey(0);

    return 0;
}
