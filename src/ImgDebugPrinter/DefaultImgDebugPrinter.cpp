//
// Created by osboxes on 11/12/18.
//

#include "ImgDebugPrinter.h"

void DefaultImgDebugPrinter::preShowImage(std::string windowName) {
}

void DefaultImgDebugPrinter::showImage(std::string windowName, int index, cv::Mat mat) {
    gameFrames.push(mat);
}
