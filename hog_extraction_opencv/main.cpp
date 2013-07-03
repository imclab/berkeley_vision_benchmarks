
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "helpers.h"
using namespace std;
using namespace cv;

vector<float> wrappedCvHog(cv::Mat img)
{
    //magic parameters -- see experienceopencv.blogspot.com/2011/02/hog-descriptor.html
    bool gamma_corr = true;
    cv::Size win_size(img.rows, img.cols);
    cv::Size block_size(16,16);
    cv::Size block_stride(8,8);
    cv::Size cell_size(8,8);
    int nOri = 15;

    //off-the-shelf version was from opencv/opencv/samples/ocl/hog.cpp (search for cpu_hog) 
    cv::HOGDescriptor d(win_size, block_size, block_stride, cell_size, nOri, 1, -1,
                              cv::HOGDescriptor::L2Hys, 0.2, gamma_corr, cv::HOGDescriptor::DEFAULT_NLEVELS); //configure HOG extractor

    vector<float> features;
    vector<cv::Point> locations;
    d.compute(img, features, cv::Size(0,0), cv::Size(0,0), locations); //run HOG extractor
    printf("features.size() = %d \n", (int)features.size());
    return features;
}

void benchmarkOpenCvHOG()
{
    Mat img = imread("Lena.pgm");
    cv::cvtColor(img, img, CV_RGB2GRAY);   

    double start = read_timer();
    wrappedCvHog(img); 
    double responseTime = read_timer() - start;
    printf("OpenCV HOG time = %f \n", responseTime);
}

int main (int argc, char **argv)
{
    benchmarkOpenCvHOG();    

    //TODO: visualize HOG features

    return 0;
}
