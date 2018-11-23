//
// Created by Konstantin Gredeskoul on 5/14/17.
//

#ifndef CMAKE_PIPSDETECTOR_H
#define CMAKE_PIPSDETECTOR_H

#include <iostream>

#include "../ImgDebugPrinter/ImgDebugPrinter.h"

using namespace std;

static const int DOMINO_PIECE_AREA_MIN = 18000;
static const int DOMINO_PIECE_AREA_MAX = 100353;

class PipsDetector {
public:
    PipsDetector(AbstractImgDebugPrinter* printer);
    int detect(cv::Mat mat);
protected:
    unsigned long countPips(cv::Mat piece);
    AbstractImgDebugPrinter* _printer;
public:
    PipsDetector();
};

class PipsDetectorFactory {
public:
    PipsDetector* createDefaultPipsDetector() {
        AbstractImgDebugPrinter* printer = IImgDebugPrinterFactory().getPrinter();
        PipsDetector* pipsdetector = new PipsDetector(printer);
        return pipsdetector;
    }
};

#endif //CMAKE_PIPSDETECTOR_H
