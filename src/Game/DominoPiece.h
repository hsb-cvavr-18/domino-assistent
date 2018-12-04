//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_STONE_H
#define PROJECT_STONE_H

#include <list>
#include <opencv2/core.hpp>
#include "DominoHalf.h"

struct Points{
    cv::Point pointA;
    cv::Point pointB;
    cv::Point pointC;
    cv::Point pointD;
};

class DominoPiece {

private:
    DominoHalf dominoHalfA ,
               dominoHalfB ;

public:

    DominoPiece(DominoHalf dominoHalfA, DominoHalf dominoHalfB) : dominoHalfA(dominoHalfA), dominoHalfB(dominoHalfB) {}


    bool isDoubletsStone() {
        return dominoHalfA.getNumber()==dominoHalfB.getNumber();
    }

    bool isNormalStone() {
        return !isDoubletsStone();
    }

};


#endif //PROJECT_STONE_H
