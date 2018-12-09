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
    DominoHalf(cv::RotatedRect rect, int number);

    int getNumber() const;

    void setNumber(int number);

    const cv::RotatedRect &getRect() const;

    void setRect(cv::RotatedRect rect);
};


#endif //PROJECT_HALFSTONE_H
