#include "colorhistogram.h"
#include "histogram1d.h"

ColorHistogram::ColorHistogram()
{
    // Prepare arguments for a color histogram
    histSize[0]= histSize[1]= histSize[2]= 256;
    hranges[0]= 0.0; // BRG range
    hranges[1]= 255.0;
    ranges[0]= hranges; // all channels have the same range
    ranges[1]= hranges;
    ranges[2]= hranges;
    channels[0]= 0; // the three channels
    channels[1]= 1;
    channels[2]= 2;
}

cv::MatND ColorHistogram::getHistogram(const cv::Mat &image) {
    cv::MatND hist;
    // Compute histogram
    cv::calcHist(&image,
                 1, // histogram of 1 image only
                 channels, // the channel used
                 cv::Mat(), // no mask is used
                 hist, // the resulting histogram
                 3, // it is a 3D histogram
                 histSize, // number of bins
                 ranges // pixel value range
                 );
    return hist;
}

cv::SparseMat ColorHistogram::getSparseHistogram(const cv::Mat &image) {
    cv::SparseMat hist(3,histSize,CV_32F);
    // Compute histogram
    cv::calcHist(&image,
                 1, // histogram of 1 image only
                 channels, // the channel used
                 cv::Mat(), // no mask is used
                 hist, // the resulting histogram
                 3, // it is a 3D histogram
                 histSize, // number of bins
                 ranges // pixel value range
                 );
    return hist;
}

std::vector<cv::MatND> ColorHistogram::getChannelHistogram(const cv::Mat &image) {
    //Split color image by BGR channel and draw histogram per BGR(A)
    std::vector<cv::Mat> planes;
    cv::split(image,planes);
    int channels = planes.size();
    CV_Assert(channels < 5); //BGRA channels most
    Histogram1D h;
    std::vector<cv::MatND> chanHistoVec;

    for(int ch=0; ch<channels; ch++) {
        cv::Mat matChannel = planes[ch];
        cv::MatND hist = h.getHistogram(matChannel);
        chanHistoVec.push_back(hist);
    }
    return chanHistoVec;
}

cv::Mat ColorHistogram::getChannelHistogramImage(const cv::Mat &image) {
    std::vector<cv::MatND> chanHistoVec = getChannelHistogram(image);
    int rng = histSize[0];
    cv::Mat chanHistImage(rng,rng,CV_8UC3,cv::Scalar(255,255,255));
    //draw histogram lines per each channel in B/G/R
    int channels = chanHistoVec.size();

    //highest position value at 90% height
    int hpv = static_cast<int>(0.9*rng);

    //decide max histogram value considering all channels
    double minVal;
    double maxVal;
    double maxmax=0;
    for(int i=0; i<channels; i++) {
        cv::minMaxIdx(chanHistoVec[i],&minVal,&maxVal,0,0);
        if(maxmax<maxVal) maxmax=maxVal;
    }

    //BGR pure color scalar
    cv::Scalar colors[] = {cv::Scalar(255,0,0),cv::Scalar(0,255,0),cv::Scalar(0,0,255),cv::Scalar(128,128,128)};
    //draw line per each channel
    for(int i=0; i<channels; i++) {
        cv::MatND chanHisto = chanHistoVec[i];
        int len = chanHisto.rows;
        float *data = chanHisto.ptr<float>(0);
        float binPrev = data[0];
        int indensityPrev = static_cast<int>(binPrev * hpv / maxmax);
        for(int j=1; j<len; j++) {
            float binCur = data[j];
            int indensityCur = static_cast<int>(binCur * hpv / maxmax);
            //solid line other than bar
            cv::line(chanHistImage,
                     cv::Point(j,rng-indensityPrev),
                     cv::Point(j,rng-indensityCur),
                     colors[i]);
            binPrev = binCur;
            indensityPrev = indensityCur;
        }
    }
    return chanHistImage;
}

cv::MatND ColorHistogram::getHueHistogram(const cv::Mat &image, int minSatuation) {
    cv::MatND hist;
    //Assuming input is BGR, change it to HSV color space
    cv::Mat hsv;
    cv::cvtColor(image,hsv,CV_BGR2HSV);
    cv::Mat mask;
    if(minSatuation > 0) {
        //split to 3 channels: H/S/V
        std::vector<cv::MatND> v;
        cv::split(hsv,v);
        //prepare mask to ignore low satuation
        cv::threshold(v[1],mask,minSatuation,255,cv::THRESH_BINARY);
    }

    //prepare para. for HSV histogram calculation
    hranges[1] = 180.0;
    channels[0] = 0;
    cv::calcHist(&hsv,1,channels,mask,hist,1,histSize,ranges);
    //restore hranges to [0,255]
    hranges[1] = 255.0;
    return hist;
}

cv::Mat ColorHistogram::colorReduce(const cv::Mat &image, int div) {
    cv::Mat result;
    result.create(image.rows, image.cols, image.type());
    int nl= image.rows; // number of lines
    int nc= image.cols * image.channels();
    if (image.isContinuous())
    {
        // then no padded pixels
        nc= nc*nl;
        nl= 1;  // it is now a 1D array
    }
    // this loop is executed only once
    // in case of continuous images
    for (int j=0; j<nl; j++) {
        const uchar* data= image.ptr<uchar>(j);
        uchar* rdata = result.ptr<uchar>(j);
        for (int i=0; i<nc; i++) {
            // process each pixel ---------------------
            rdata[i]= data[i]/div*div + div/2;
            // end of pixel processing ----------------
        } // end of line
    }
    return result;
}

