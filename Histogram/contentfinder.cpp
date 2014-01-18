#include "contentfinder.h"

cv::Mat ContentFinder::find(const cv::Mat& image,
             float minValue, float maxValue,
             int *channels, int dim) {
    cv::Mat result;
    //back projection based on histogram
    if( !histogram.empty() ) {
        hranges[0] = minValue;
        hranges[1] = maxValue;
        for(int i =0; i<dim; i++)
            this->channels[i] = channels[i];
        cv::calcBackProject(&image,1,channels,histogram,result,ranges,255.0);

        //threshold?
        if(threshold >0) {
            cv::threshold(result,result,threshold,255.0,cv::THRESH_BINARY);
        }
    }
    return result;
}
