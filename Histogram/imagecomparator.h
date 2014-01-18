#ifndef IMAGECOMPARATOR_H
#define IMAGECOMPARATOR_H

#include <opencv2/opencv.hpp>
#include "colorhistogram.h"

class ImageComparator {
private:
    cv::Mat reference;
    cv::Mat input;
    cv::MatND refH;
    cv::MatND inputH;
    ColorHistogram hist;
    int div;
public:
    ImageComparator() : div(32) {
    }

    // Color reduction factor
    // The comparison will be made on images with
    // color space reduced by this factor in each dimension
    void setColorReduction( int factor) {
        div= factor;
    }
    int getColorReduction() {
        return div; }

    void setReferenceImage(const cv::Mat& image) {
        reference= hist.colorReduce(image,div);
        refH= hist.getHistogram(reference);
    }

    double compare(const cv::Mat& image) {
        input= hist.colorReduce(image,div);
        inputH= hist.getHistogram(input);
        return cv::compareHist(
                    refH,inputH,CV_COMP_INTERSECT);
    }
};

#endif // IMAGECOMPARATOR_H
