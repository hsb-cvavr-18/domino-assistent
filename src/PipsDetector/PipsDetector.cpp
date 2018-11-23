//
// Created by Konstantin Gredeskoul on 5/14/17.
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

unsigned long PipsDetector::countPips(cv::Mat half) {

    // resize
    cv::resize(half, half, cv::Size(150, 150));

    // convert to grayscale
    //cvtColor(half, half, CV_BGR2GRAY);

    // threshold
    cv::threshold(half, half, 150, 255, cv::THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite("domino_pips_bin.jpg", half);
    // floodfill
    cv::floodFill(half, cv::Point(0, 0), cv::Scalar(255));
    cv::floodFill(half, cv::Point(0, 149), cv::Scalar(255));
    cv::floodFill(half, cv::Point(149, 0), cv::Scalar(255));
    cv::floodFill(half, cv::Point(149, 149), cv::Scalar(255));
    cv::imwrite("domino_pips_flood.jpg", half);
    // show
    cv::imwrite("processed.jpg", half);

    // search for blobs
    cv::SimpleBlobDetector::Params params;

    cv::Mat halfTemp;
    half.copyTo(halfTemp);
    cv::GaussianBlur(half, halfTemp, cv::Size(9,9), 3.3);
    half = halfTemp;
    cv::imwrite("domino_pips_gauss.jpg", half);

    // filter by interia defines how elongated a shape is.
    //params.filterByInertia = true;
    //params.minInertiaRatio = 0.5;

    params.filterByColor = true;
    params.blobColor = 0;
    params.minArea = 200;

    // will hold our keyponts
    std::vector<cv::KeyPoint> keypoints;

    // create new blob detector with our parameters
    cv::Ptr<cv::SimpleBlobDetector> blobDetector = cv::SimpleBlobDetector::create(params);

    // detect blobs
    blobDetector->detect(half, keypoints);

    // return number of pips
    return keypoints.size();
}

int PipsDetector::detect(cv::Mat img) {
    return static_cast<int>(countPips(img));
}