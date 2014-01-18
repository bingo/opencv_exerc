#include <iostream>
#include <sstream>

#include <QCoreApplication>

#include "histogram1d.h"
#include "colorhistogram.h"
#include "contentfinder.h"
#include "imagecomparator.h"

using namespace std;

void testGrayHistogram(const cv::Mat &image) {
    Histogram1D h;
    cv::Mat histImg = h.getHistogramImage(image);
    cv::imshow("Gray Histogram",histImg);
}

void testColorHistogram(const cv::Mat &cImage) {
    ColorHistogram ch;
    cv::Mat colorHist = ch.getChannelHistogramImage(cImage);
    cv::imshow("Color Histogram",colorHist);
}

void testReverseImage(const cv::Mat image) {
    //make lookup table - gray
    int dim(256);
    cv::Mat lut(1,
                &dim,
                CV_8U);
    for(int i=0; i<dim; i++) {
        lut.at<uchar>(i) = 255 - i;
    }

    Histogram1D h;
    cv::Mat revImage = h.applyLookUp(image, lut);
    cv::imshow("Reverse", revImage);
}

void testStrechImage(const cv::Mat &image, int minVal=0) {
    Histogram1D h;
    cv::Mat strcImage = h.stretch(image,minVal);
    cv::imshow("Streched", strcImage);
    cv::Mat histo = h.getHistogramImage(strcImage);
    cv::imshow("Streched Histogram",histo);
}

void testEqualizerImage(const cv::Mat &image) {
    Histogram1D h;
    cv::Mat eqlImage = h.equalize(image);
    cv::imshow("Equalizer",eqlImage);
    cv::Mat histo = h.getHistogramImage(eqlImage);
    cv::imshow("Equalizer Histogram",histo);
}

void testBackProjection(const cv::Mat &image, const cv::Rect &region) {
    Histogram1D h;
    cv::Mat bpImage = h.backProject(image,region);
    //highlight region area
    cv::rectangle(bpImage,region,cv::Scalar(128));
    cv::imshow("Back Projection", bpImage);
}

void testMeanShiftBackProjection(const cv::Mat &imageSrc, const cv::Rect &region, const cv::Mat &imageDst) {
    //Display src/dst image
    cv::imshow("Src Image", imageSrc);
    ContentFinder finder;
    ColorHistogram hc;
    int minSat = 65;
    cv::Mat imageROI = imageSrc(region);
    cv::MatND hist = hc.getHueHistogram(imageROI,minSat);
    finder.setHistogram(hist);
    cv::Mat hsv;
    cv::cvtColor(imageDst,hsv,CV_BGR2HSV);
    std::vector<cv::MatND> v;
    cv::split(hsv,v);
    //filter out low satuation
    cv::threshold(v[1],v[1],minSat,255.0,cv::THRESH_BINARY);
    //back projection find
    int ch[] = {0};
    cv::Mat result = finder.find(hsv,0.0,180.0,ch,1);
    // Eliminate low stauration pixels
    cv::bitwise_and(result,v[1],result);

    //meanshift algo.
    cv::Rect rect(region);
    cv::Mat imageRslt = imageDst.clone();
    cv::rectangle(imageRslt, rect, cv::Scalar(0,0,255)); //origion rect
    cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER,
                              10,0.01);
    cv::meanShift(result,rect,criteria);
    cv::rectangle(imageRslt, rect, cv::Scalar(0,255,0)); //result rect
    cv::imshow("Result Image",imageRslt);
}

void testImageCompare() {
    //test similarity sky1 and 2/3/4.jpg
    ImageComparator comp;
    cv::Mat imgRef = cv::imread("images/sky1.jpg",1);
    if(!imgRef.data) {
        return;
    }
    comp.setReferenceImage(imgRef);
    for(int i=2;i<5;i++) {
        stringstream ssfn;
        ssfn << "images/sky" << i << ".jpg";
        cv::Mat imgToCmp = cv::imread(ssfn.str(),1);
        if(!imgToCmp.data) {
            continue;
        }
        double result = comp.compare(imgToCmp);
        //draw "Similarity" rate on window caption
        stringstream caption;
        caption << "Similarity:" << result;
        cv::imshow(caption.str(), imgToCmp);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc,argv);
    string fileName;
    if(argc < 2) {
        cout << "Usage: Histogram <img_file>! Default image loaded" << endl;
        fileName = "images/sky1.jpg";
    }else{
        fileName = argv[1];
    }
    cv::Mat image = cv::imread(fileName,0);
    cv::Mat cImage = cv::imread(fileName,1);
    if( !image.data || !cImage.data ) {
        cout << "Unable to load image, exit..." << endl;
        return 1;
    }

    //show original image to compare
    //    cv::imshow("Origin", image);
    //    Histogram1D h;
    //    cv::Mat histImage = h.getHistogramImage(image);
    //    cv::imshow("Histogram", histImage);

    //  testGrayHistogram(image);
    //  testColorHistogram(cImage);
    //  testReverseImage(image);
    //  testStrechImage(image,128);
    //  testEqualizerImage(image);
    //  cv::Rect region(100,100,50,50);
    //  testBackProjection(image,region);
    //  cv::Mat imageSrc = cv::imread("images/baboon1.jpg", 1);
    //  cv::Rect region(110,260,35,40);
    //  cv::Mat imageDst = cv::imread("images/baboon3.jpg", 1);
    //  testMeanShiftBackProjection(imageSrc,region,imageDst);
    testImageCompare();

    cv::waitKey();

    return a.exec();
}
