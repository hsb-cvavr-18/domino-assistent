//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_HALFSTONE_H
#define PROJECT_HALFSTONE_H


#include <opencv2/core/types.hpp>
#include <exception>

class HalfStone {
private:
    cv::Point a,b;
    int number;
    //double angle;

public:
    int getNumber() const {
        return number;
    }

    void setNumber(int number) {
        number = number;
    }

    const cv::Point &getA() const {
        return a;
    }

    void setA(const cv::Point &a) {
        this->a = a;
    }

    const cv::Point &getB() const {
        return b;
    }

    void setB(const cv::Point &b) {
        this->b = b;
    }

    HalfStone(cv::Point a, cv::Point b, int number) {
        this->a=a;
        this->b=b;
        if (number>=0 && number<=6)
            this->number=number;
        else
            throw new std::exception();
    }


};


#endif //PROJECT_HALFSTONE_H
