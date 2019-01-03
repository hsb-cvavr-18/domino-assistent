//
// Created by osboxes on 11/29/18.
//

#include "DominoPiece.h"

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
    return(this->getHalfA().getNumber() == piece.getHalfA().getNumber() ||
           this->getHalfB().getNumber() == piece.getHalfB().getNumber() ||
           this->getHalfA().getNumber() == piece.getHalfB().getNumber() ||
           this->getHalfB().getNumber() == piece.getHalfA().getNumber()
    );
}

DominoPiece::DominoPiece() {

}

bool DominoPiece::operator==(DominoPiece rhs) {
    return (
            this->getHalfA()==rhs.getHalfA()  &&
            this->getHalfB()==rhs.getHalfB()) ||
           (this->getHalfA()==rhs.getHalfB()  &&
            this->getHalfB()==rhs.getHalfA());
}

std::ostream& operator<<(std::ostream &os, DominoPiece &p) {
    return os << "[" << p.getHalfA().getNumber() << "," << p.getHalfB().getNumber() << "]";
}





