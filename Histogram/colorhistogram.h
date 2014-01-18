#ifndef COLORHISTOGRAM_H
#define COLORHISTOGRAM_H

#include <vector>
#include <opencv2/opencv.hpp>

class ColorHistogram {
private:
    int histSize[3];
    float hranges[2];
    const float* ranges[3];
    int channels[3];
public:
    ColorHistogram();
    cv::MatND getHistogram(const cv::Mat &image);
    cv::SparseMat getSparseHistogram(const cv::Mat &image);
    std::vector<cv::MatND> getChannelHistogram(const cv::Mat &image);
    cv::Mat getChannelHistogramImage(const cv::Mat &image);
    //get HUE histogram
    cv::MatND getHueHistogram(const cv::Mat &image, int minSatuation = 0);
    //reduce color
    cv::Mat colorReduce(const cv::Mat &image, int div = 64);
};

#endif // COLORHISTOGRAM_H
