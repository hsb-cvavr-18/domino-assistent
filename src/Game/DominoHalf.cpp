//
// Created by osboxes on 11/29/18.
//

#include "DominoHalf.h"

DominoHalf::DominoHalf(cv::RotatedRect rect, int number) {
    this->rect=rect;
    this->number=number;
}

int DominoHalf::getNumber()  {
    return number;
}

void DominoHalf::setNumber(int number) {
    this->number = number;
}

cv::RotatedRect &DominoHalf::getRect() {
    return rect;
}

void DominoHalf::setRect(cv::RotatedRect rect) {
    DominoHalf::rect = rect;
}

DominoHalf::DominoHalf() {

}

bool DominoHalf::operator==(DominoHalf rhs) {
    return (
            this->getNumber()==rhs.getNumber()  &&
            this->getNumber()==rhs.getNumber()) ||
           (this->getNumber()==rhs.getNumber()  &&
            this->getNumber()==rhs.getNumber());
}
