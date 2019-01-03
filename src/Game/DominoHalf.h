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
    bool bl;

public:
    DominoHalf();
    DominoHalf(cv::RotatedRect rect, unsigned int number);

    bool operator==(DominoHalf rhs);

    unsigned int getNumber();
    void setNumber(unsigned int number);

    cv::RotatedRect &getRect();
    void setRect(cv::RotatedRect rect);

    bool isBlocked();
    void block();
    void unblock();
    bool isApplicableTo(DominoHalf otherHalf);
};


#endif //PROJECT_HALFSTONE_H
