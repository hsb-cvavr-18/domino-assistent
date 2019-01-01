//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_STONE_H
#define PROJECT_STONE_H

#include <list>
#include <opencv2/core.hpp>
#include "DominoHalf.h"

class DominoPiece {

private:
    DominoHalf halfA ,
               halfB ;
public:
    DominoPiece(DominoHalf dominoHalfA, DominoHalf dominoHalfB) : halfA(dominoHalfA), halfB(dominoHalfB) {}
    DominoPiece();
    bool operator==(DominoPiece rhs);
    DominoHalf &getHalfA();
    void setHalfA( DominoHalf &halfA) {DominoPiece::halfA = halfA;}

    DominoHalf &getHalfB();
    void setHalfB( DominoHalf &halfB) {DominoPiece::halfB = halfB;}

    bool isDoubletsStone();
    bool isNormalStone();
    cv::Point2f getCenter();;

    bool isApplicableTo(DominoPiece piece);
};


#endif //PROJECT_STONE_H
