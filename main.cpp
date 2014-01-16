#include <QCoreApplication>
#include <iostream>

#include "histogram1d.h"
#include "colorhistogram.h"

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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc,argv);
    if(argc < 2) {
        cout << "Usage: Histogram <img_file>!" << endl;
        return 0;
    }
    char *fileName = argv[1];
    cv::Mat image = cv::imread(fileName,0);
    cv::Mat cImage = cv::imread(fileName,1);
    if( !image.data || !cImage.data ) {
        cout << "Unable to load image, exit..." << endl;
        return 1;
    }

    //show original image to compare
    cv::imshow("Origin", image);
    Histogram1D h;
    cv::Mat histImage = h.getHistogramImage(image);
    cv::imshow("Histogram", histImage);

    //testGrayHistogram(image);
    //testColorHistogram(cImage);
    //testReverseImage(image);
    testStrechImage(image,128);

    cv::waitKey();

    return a.exec();
}
