//
// Created by osboxes on 11/12/18.
//

#include "ImgDebugPrinter.h"

void DefaultImgDebugPrinter::preShowImage(std::string windowName) {
    cv::namedWindow(windowName, cv::WINDOW_NORMAL);
}

void DefaultImgDebugPrinter::showImage(std::string windowName, int index, cv::Mat mat) {
    cv::resizeWindow(windowName, mat.cols / 2, mat.rows / 2);
    cv::imshow(windowName, mat);
    /*
    cv::namedWindow(windowName,cv::WINDOW_NORMAL);
    cv::imshow(windowName, mat);
    cv::resizeWindow(windowName, 800, 600);*/

    cv::waitKey();
}
