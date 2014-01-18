#ifndef CONTENTFINDER_H
#define CONTENTFINDER_H

#include <opencv2/opencv.hpp>

class ContentFinder {
private:
    float hranges[2];
    const float* ranges[3];
    int channels[3];
    float threshold;
    cv::MatND histogram;
public:
    ContentFinder() : threshold(-1.0f) {
        ranges[0]= hranges; // all channels have same range
        ranges[1]= hranges;
        ranges[2]= hranges;
    }

    //Threshold to be applied
    void setThreshold(float t) { threshold = t; }
    float getThreshold() { return threshold; }
    //Histogram para.
    void setHistogram(const cv::MatND &h) {
        histogram = h;
        cv::normalize(histogram,histogram,1.0);
    }

    //find histogram backprojection mapping
    cv::Mat find(const cv::Mat& image,
                 float minValue, float maxValue,
                 int *channels, int dim);
};

#endif // CONTENTFINDER_H
