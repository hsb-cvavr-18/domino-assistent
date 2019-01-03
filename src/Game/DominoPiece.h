//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_STONE_H
#define PROJECT_STONE_H

#include <list>
#include <opencv2/core.hpp>
#include "DominoHalf.h"

class DominoPiece {

private:
    DominoHalf halfA ,
               halfB ;

public:
    DominoPiece();
    DominoPiece(DominoHalf dominoHalfA, DominoHalf dominoHalfB);

    bool operator==(DominoPiece rhs);

    DominoHalf &getHalfA();
    DominoHalf &getHalfB();

    bool isDoubletsStone();
    bool isNormalStone();
    cv::Point2f getCenter();;

    bool isApplicableTo(DominoPiece piece);
    void block(int num);

    friend std::ostream& operator<<(std::ostream &os, DominoPiece &p);
    cv::Point2f getOffset();
};


#endif //PROJECT_STONE_H
