//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_STONE_H
#define PROJECT_STONE_H

#include <list>
#include <opencv2/core.hpp>
#include "HalfStone.h"

struct Points{
    cv::Point pointA;
    cv::Point pointB;
    cv::Point pointC;
    cv::Point pointD;
};

class Stone {

private:
    HalfStone halfStoneA = HalfStone(cv::Point(), cv::Point(), 0),
              halfStoneB = HalfStone(cv::Point(), cv::Point(), 0);
    //cv::Point center;

public:

    Stone(HalfStone a, HalfStone b) {
        halfStoneA=a;
        halfStoneB=b;
    }

    Points getPoints() {
        Points points;
        points.pointA=halfStoneA.getA();
        points.pointB=halfStoneA.getB();
        points.pointC=halfStoneB.getA();
        points.pointD=halfStoneB.getB();
        return points;
    }

    cv::Point getCenter_Calculated() {
        int x = (halfStoneA.getA().x +
                 halfStoneA.getB().x +
                 halfStoneB.getA().x +
                 halfStoneB.getB().x)/4;

        int y = (halfStoneA.getA().y +
                 halfStoneA.getB().y +
                 halfStoneB.getA().y +
                 halfStoneB.getB().y)/4;

        return cv::Point(x,y);
    }

    bool isDoubletsStone() {
        return halfStoneA.getNumber()==halfStoneB.getNumber();
    }

    bool isNormalStone() {
        return !isDoubletsStone();
    }

};


#endif //PROJECT_STONE_H
