#include "ImgDebugPrinter.h"
#include <iostream>

void StubImgDebugPrinter::preShowImage(std::string windowName) {
    std::cout << "STUB: preShowImage: " << windowName << std::endl;
}

void StubImgDebugPrinter::showImage(std::string windowName, int index, cv::Mat mat) {
    std::cout << "STUB: showImage: " << windowName << std::endl;
}
