#include <utility>

#include <utility>

//
// Created by osboxes on 11/29/18.
//

#include "DominoPiece.h"

DominoPiece::DominoPiece() = default;

DominoPiece::DominoPiece(DominoHalf dominoHalfA, DominoHalf dominoHalfB) {
    halfA= std::move(dominoHalfA);
    halfB= std::move(dominoHalfB);
}

bool DominoPiece::operator==(DominoPiece rhs) {
    return (
            this->getHalfA()==rhs.getHalfA()  &&
            this->getHalfB()==rhs.getHalfB()) ||
           (this->getHalfA()==rhs.getHalfB()  &&
            this->getHalfB()==rhs.getHalfA());
}

DominoHalf &DominoPiece::getHalfA() {return halfA;}
DominoHalf &DominoPiece::getHalfB() {return halfB;}

bool DominoPiece::isDoubletsStone() {return halfA.getNumber()==halfB.getNumber();}
bool DominoPiece::isNormalStone() {return !isDoubletsStone();}

cv::Point2f DominoPiece::getCenter() {
    return (halfA.getRect().center + halfB.getRect().center) / 2.0;
}

bool DominoPiece::isApplicableTo(DominoPiece piece) {
    return this->getHalfA().isApplicableTo(piece.getHalfA()) ||
           this->getHalfA().isApplicableTo(piece.getHalfB()) ||
           this->getHalfB().isApplicableTo(piece.getHalfA()) ||
           this->getHalfB().isApplicableTo(piece.getHalfB());
}

void DominoPiece::block(int num) {
    if(halfA.getNumber()==num && !halfA.isBlocked())
        halfA.block();
    else if (halfB.getNumber()==num && !halfB.isBlocked())
        halfB.block();

}


