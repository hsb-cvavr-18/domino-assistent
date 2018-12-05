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

    const DominoHalf &getHalfA() const {return halfA;}
    void setHalfA(const DominoHalf &halfA) {DominoPiece::halfA = halfA;}

    const DominoHalf &getHalfB() const {return halfB;}
    void setHalfB(const DominoHalf &halfB) {DominoPiece::halfB = halfB;}

    bool isDoubletsStone() {return halfA.getNumber()==halfB.getNumber();}
    bool isNormalStone() {return !isDoubletsStone();}
    cv::Point2f getCenter() {
        return (halfA.getRect().center + halfB.getRect().center) / 2.0;
    };

    bool isApplicableTo(DominoPiece piece) {
        return(this->getHalfA().getNumber() == piece.getHalfA().getNumber() ||
               this->getHalfB().getNumber() == piece.getHalfB().getNumber() ||
               this->getHalfA().getNumber() == piece.getHalfB().getNumber() ||
               this->getHalfB().getNumber() == piece.getHalfA().getNumber()
        );
    }
};


#endif //PROJECT_STONE_H
