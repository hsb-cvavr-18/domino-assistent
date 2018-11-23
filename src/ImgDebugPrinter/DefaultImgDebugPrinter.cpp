//
// Created by osboxes on 11/12/18.
//

#include "ImgDebugPrinter.h"

void DefaultImgDebugPrinter::preShowImage(std::string windowName) {
    cv::namedWindow(windowName, true);
}

void DefaultImgDebugPrinter::showImage(std::string windowName, int index, cv::Mat mat) {
    cv::imshow(windowName, mat); cv::waitKey(0);
}
