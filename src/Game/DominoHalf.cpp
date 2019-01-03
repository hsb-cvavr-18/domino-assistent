//
// Created by osboxes on 11/29/18.
//

#include "DominoHalf.h"

DominoHalf::DominoHalf() {
    isBlocked=false;
}

DominoHalf::DominoHalf(cv::RotatedRect rect, unsigned int number) {
    this->rect = rect;
    this->number = number;
    isBlocked=false;
}

bool DominoHalf::operator==(DominoHalf rhs) {
    return (
                   this->getNumber()==rhs.getNumber()  &&
                   this->getNumber()==rhs.getNumber()) ||
           (this->getNumber()==rhs.getNumber()  &&
            this->getNumber()==rhs.getNumber());
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

void DominoHalf::block() {
    isBlocked=true;
}

void DominoHalf::unblock() {
    isBlocked=false;
}

