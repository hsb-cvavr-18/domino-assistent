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

cv::Point2f DominoPiece::getOffset() {
    //taking offset of halfA. Offset of Half A and be MUST be equal.
    return this->halfA.getOffset();
}

bool DominoPiece::isApplicableTo(DominoPiece piece) {
    if(this->isNormalStone()) {
        return this->getHalfA().isApplicableTo(piece.getHalfA()) ||
               this->getHalfA().isApplicableTo(piece.getHalfB()) ||
               this->getHalfB().isApplicableTo(piece.getHalfA()) ||
               this->getHalfB().isApplicableTo(piece.getHalfB());
    }else{
        this->getHalfA().getNumber()==piece.getHalfA().getNumber() ||
        this->getHalfA().getNumber()==piece.getHalfB().getNumber() ||
        this->getHalfB().getNumber()==piece.getHalfA().getNumber() ||
        this->getHalfB().getNumber()==piece.getHalfB().getNumber() ||
    }
}

void DominoPiece::block(int num) {
    if(halfA.getNumber()==num && !halfA.isBlocked())
        halfA.block();
    else if (halfB.getNumber()==num && !halfB.isBlocked())
        halfB.block();

}


std::ostream& operator<<(std::ostream &os, DominoPiece &p) {
    return os << "[" << p.getHalfA().getNumber() << "," << p.getHalfB().getNumber() << "]";
}





