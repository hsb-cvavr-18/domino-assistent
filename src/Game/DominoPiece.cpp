//
// Created by osboxes on 11/29/18.
//

#include "DominoPiece.h"

const DominoHalf &DominoPiece::getHalfA() const {return halfA;}

const DominoHalf &DominoPiece::getHalfB() const {return halfB;}

bool DominoPiece::isDoubletsStone() {return halfA.getNumber()==halfB.getNumber();}

bool DominoPiece::isNormalStone() {return !isDoubletsStone();}

cv::Point2f DominoPiece::getCenter() {
    return (halfA.getRect().center + halfB.getRect().center) / 2.0;
}

bool DominoPiece::isApplicableTo(DominoPiece piece) {
    return(this->getHalfA().getNumber() == piece.getHalfA().getNumber() ||
           this->getHalfB().getNumber() == piece.getHalfB().getNumber() ||
           this->getHalfA().getNumber() == piece.getHalfB().getNumber() ||
           this->getHalfB().getNumber() == piece.getHalfA().getNumber()
    );
}
