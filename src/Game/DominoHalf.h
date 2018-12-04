//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_HALFSTONE_H
#define PROJECT_HALFSTONE_H


#include <opencv2/core/types.hpp>
#include <exception>

class DominoHalf {
private:
    cv::RotatedRect rect;
    int number;

public:
    DominoHalf(cv::RotatedRect rect, int number) {
        this->rect=rect;
        this->number=number;
    }

    int getNumber() const {
        return number;
    }

    void setNumber(int number) {
        number = number;
    }

    const cv::RotatedRect &getRect() const {
        return rect;
    }

    void setRect(cv::RotatedRect rect) {
        DominoHalf::rect = rect;
    }
};


#endif //PROJECT_HALFSTONE_H
