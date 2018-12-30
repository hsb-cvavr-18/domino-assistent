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
    unsigned int number;

public:
    DominoHalf(cv::RotatedRect rect, unsigned int number);

    DominoHalf();

    int getNumber();

    void setNumber(unsigned int number);

    cv::RotatedRect &getRect();

    void setRect(cv::RotatedRect rect);
};


#endif //PROJECT_HALFSTONE_H
