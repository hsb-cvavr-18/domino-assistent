//
// Created by osboxes on 11/29/18.
//

#include "DominoHalf.h"

DominoHalf::DominoHalf(cv::RotatedRect rect, int number) {
    this->rect=rect;
    this->number=number;
}

int DominoHalf::getNumber() const {
    return number;
}

void DominoHalf::setNumber(int number) {
    this->number = number;
}

const cv::RotatedRect &DominoHalf::getRect() const {
    return rect;
}

void DominoHalf::setRect(cv::RotatedRect rect) {
    DominoHalf::rect = rect;
}
