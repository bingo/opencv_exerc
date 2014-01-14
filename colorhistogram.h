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
    std::vector<cv::MatND> getChannelHistogram(const cv::Mat &image);
    cv::Mat getChannelHistogramImage(const cv::Mat &image);
};

#endif // COLORHISTOGRAM_H
