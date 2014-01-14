#include <QCoreApplication>
#include <iostream>

#include "histogram1d.h"
#include "colorhistogram.h"

using namespace std;

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
    if( !image.data ) {
        cout << "Unable to load image, exit..." << endl;
        return 1;
    }

    Histogram1D h;
    cv::Mat histImg = h.getHistogramImage(image);
    ColorHistogram ch;
    cv::Mat colorHist = ch.getChannelHistogramImage(cImage);

    cv::imshow("Gray Histogram",histImg);
    cv::imshow("Color Histogram",colorHist);
    cv::waitKey();

    return a.exec();
}
