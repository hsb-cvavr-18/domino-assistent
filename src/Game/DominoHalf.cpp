//
// Created by osboxes on 11/29/18.
//

#include "DominoHalf.h"

DominoHalf::DominoHalf(cv::RotatedRect rect, unsigned int number, cv::Point2f offset) {
    this->rect = rect;
    this->number = number;
    this->offset = offset;
}

unsigned int DominoHalf::getNumber() {
    return number;
}

void DominoHalf::setNumber(unsigned int number) {
    this->number = number;
}

cv::RotatedRect &DominoHalf::getRect() {
    return rect;
}

void DominoHalf::setRect(cv::RotatedRect rect) {
    DominoHalf::rect = rect;
}

cv::Point2f DominoHalf::getOffset(){
    return this->offset;
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
