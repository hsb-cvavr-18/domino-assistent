//
// Created by osboxes on 11/12/18.
//

// std lib
#include <iostream>
#include <sstream>

// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

#include "PipsDetector.h"

PipsDetector::PipsDetector(AbstractImgDebugPrinter* printer) : _printer(printer) {}

unsigned int PipsDetector::countPips(cv::Mat piece) {
    int r = (rand() % 100) + 1; //testing
    std::string s = std::to_string(r);
    //cv::imwrite("domino_pips_in"+s+".jpg", piece);

    // resize
    cv::resize(piece, piece, cv::Size(150, 150));

    //Brighness and Contrast Correction
    int avgBrightness = mean(piece,cv::Mat())[0];//sc_avg[0];
    const int maxBrightness = 256;
    double alpha = maxBrightness/avgBrightness;
    int beta = 0;
    cv::Mat new_image = cv::Mat::zeros( piece.size(), piece.type() );
    piece.convertTo(piece, -1, alpha, beta);

    // convert to grayscale
    //cvtColor(piece, piece, CV_BGR2GRAY);
    //cv::imwrite("domino_pips_resized_and_optimized"+s+".jpg", piece);
    // threshold
    cv::threshold(piece, piece, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    //cv::imwrite("domino_pips_bin"+s+".jpg", piece);
    // floodfill
    cv::floodFill(piece, cv::Point(0, 0), cv::Scalar(255));
    cv::floodFill(piece, cv::Point(0, 149), cv::Scalar(255));
    cv::floodFill(piece, cv::Point(149, 0), cv::Scalar(255));
    cv::floodFill(piece, cv::Point(149, 149), cv::Scalar(255));
    //cv::imwrite("domino_pips_flood"+s+".jpg", piece);
    //white boarder around image
    copyMakeBorder( piece, piece, 10, 10, 10, 10, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

    //cv::imwrite("domino_pips_border"+s+".jpg", piece);

    // search for blobs
    cv::SimpleBlobDetector::Params params;

    cv::Mat halfTemp;
    piece.copyTo(halfTemp);
    cv::GaussianBlur(piece, halfTemp, cv::Size(9,9), 3.3);
    piece = halfTemp;
    //cv::imwrite("domino_pips_gauss"+s+".jpg", piece);

    // filter by interia defines how elongated a shape is.
    //params.filterByInertia = true;
    //params.minInertiaRatio = 0.5;

    params.filterByColor = true;
    params.blobColor = 0;
    params.minArea = 400;

    // will hold our keyponts
    std::vector<cv::KeyPoint> keypoints;

    // create new blob detector with our parameters
    cv::Ptr<cv::SimpleBlobDetector> blobDetector = cv::SimpleBlobDetector::create(params);

    // detect blobs
    blobDetector->detect(piece, keypoints);

    // return number of pips
    return keypoints.size();
}

unsigned int PipsDetector::detect(cv::Mat piece) {
    return countPips(piece);
}