#include <utility>

//
// Created by osboxes on 11/29/18.
//

#include "DominoHalf.h"

DominoHalf::DominoHalf() {
    bl=false;
}

DominoHalf::DominoHalf(cv::RotatedRect rect, unsigned int number, cv::Point2f offset) {
    this->rect = rect;
    this->rect = std::move(rect);
    this->number = number;
    bl=false;
    this->offset = offset;
}

bool DominoHalf::operator==(DominoHalf rhs) {
    return (this->getNumber()==rhs.getNumber()  &&
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
    DominoHalf::rect = std::move(rect);
}

void DominoHalf::block() {
    bl=true;
}
cv::Point2f DominoHalf::getOffset(){
    return this->offset;
}

void DominoHalf::unblock() {
    bl=false;
}

bool DominoHalf::isBlocked() {
    return bl;
}

bool DominoHalf::isApplicableTo(DominoHalf otherHalf) {
    return !this->isBlocked() && !otherHalf.isBlocked() && number == otherHalf.getNumber();
}

