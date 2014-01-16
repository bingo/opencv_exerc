#ifndef HISTOGRAM1D_H
#define HISTOGRAM1D_H

#include <opencv2/opencv.hpp>

class Histogram1D
{
private:
    int histSize[1]; // number of bins
    float hranges[2]; // min and max pixel value
    const float* ranges[1];
    int channels[1]; // only 1 channel used here

public:
    Histogram1D();

    // Computes the 1D histogram.
    cv::MatND getHistogram(const cv::Mat &image);

    // Get histogram image
    cv::Mat getHistogramImage(const cv::Mat &image);

    //Apply to lookup table
    cv::Mat applyLookUp(const cv::Mat& image, // input image
                        const cv::Mat& lookup);// 1x256 uchar matrix

    //Strech histogram to cover full range [0,255]
    cv::Mat stretch(const cv::Mat &image, int minValue=0);
};

#endif // HISTOGRAM1D_H
