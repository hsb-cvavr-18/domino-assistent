//
// Created by Konstantin Gredeskoul on 10/7/18.
//

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H


// std lib
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#include "DominoLib/DominoLib.h"
#include "ImgDebugPrinter/ImgDebugPrinter.h"
#include "PipsDetector/PipsDetector.h"
#include "ImageHandler/I_ImageHandler.h"
#include "ImageHandler/ImageHandlerFactory.h"
// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>

#include "Game/PlayGround.h"
#include "DominoLib/DominoCV.h"
#include "ImageClipping/ImageClippingFactory.h"
#include "ImageClipping/ImageClipping.h"
#include "DominoLib/SafeQueue.h"

void waitForUserInput() {
    string cinBuff;
    std::cout << "Enter key to take next img" << std::endl;
    std::getline(std::cin, cinBuff);
}

void putLabel(cv::Mat &im, const std::string label, const cv::Point &pos) {
    int fontface = cv::FONT_HERSHEY_COMPLEX;
    double scale = 2.0;
    int thickness = 2;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Mat rect;
    cv::Mat roi = im(cv::Rect(pos + cv::Point(0, baseline), pos + cv::Point(text.width, -text.height)));
    cv::Mat color(roi.size(), CV_8UC3, cv::Scalar(255,255,255));
    double alpha = 0.3;
    cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
    cv::putText(im, label, pos, fontface, scale, CV_RGB(255,255,255), thickness, 8);
}


#endif //PROJECT_MAIN_H
