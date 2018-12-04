//
// Created by osboxes on 11/12/18.
//

#ifndef CMAKE_PIPSDETECTOR_H
#define CMAKE_PIPSDETECTOR_H

#include <iostream>

#include "../ImgDebugPrinter/ImgDebugPrinter.h"

using namespace std;

static const int DOMINO_PIECE_AREA_MIN = 5000;
static const int DOMINO_PIECE_AREA_MAX = 100353;

class PipsDetector {
public:
    PipsDetector(AbstractImgDebugPrinter* printer);
    unsigned int detect(cv::Mat piece);
protected:
    /**
     * Count pips of image of a piece.
     * @param piece The piece must be black and the pips must be white. The image must be gray-colored.
     * @return The number of found pips.
     */
    unsigned int countPips(cv::Mat piece);
    AbstractImgDebugPrinter* _printer;
};

class PipsDetectorFactory {
public:
    PipsDetector* createDefaultPipsDetector() {
        AbstractImgDebugPrinter* printer = IImgDebugPrinterFactory().getPrinter();
        PipsDetector* pipsDetector = new PipsDetector(printer);
        return pipsDetector;
    }
};

#endif //CMAKE_PIPSDETECTOR_H
