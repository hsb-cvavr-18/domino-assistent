//
// Created by osboxes on 11/12/18.
//

#include <opencv2/core.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <Game.h>
#include <DominoPiece.h>
#include <opencv2/imgcodecs.hpp>

#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace testing;

#define VS vector<string>
#define VI vector<long long>


class GameTest : public ::testing::Test {

protected:
    virtual void SetUp() {
    };

    virtual void TearDown() {
    };

    virtual void verify(int index) {

    }
};

DominoPiece getDominoPieceWith(int a, int b) {
    DominoHalf dominoHalfA(cv::RotatedRect(), a);
    DominoHalf dominoHalfB(cv::RotatedRect(), b);
    DominoPiece dominoPiece(dominoHalfA, dominoHalfB);
    return dominoPiece;
}

TEST_F(GameTest, aDubletPieceIsADubletPiece) {
    DominoPiece piece = getDominoPieceWith(2, 2);
    EXPECT_THAT(piece.isDoubletsStone(), Eq(true));
}

TEST_F(GameTest, aNoneDubletPieceIs_Not_ADubletPiece) {
    DominoPiece piece = getDominoPieceWith(1, 2);
    EXPECT_THAT(piece.isDoubletsStone(), Eq(false));
}

TEST_F(GameTest, a1And2IsApplicableToA1And6) {
    DominoPiece piece = getDominoPieceWith(1, 2);
    DominoPiece piece2 = getDominoPieceWith(1, 6);
    EXPECT_THAT(piece.isApplicableTo(piece2), Eq(true));

    piece = getDominoPieceWith(2, 1);
    piece2 = getDominoPieceWith(1, 6);
    EXPECT_THAT(piece.isApplicableTo(piece2), Eq(true));

    piece = getDominoPieceWith(2, 1);
    piece2 = getDominoPieceWith(6, 1);
    EXPECT_THAT(piece.isApplicableTo(piece2), Eq(true));

    piece = getDominoPieceWith(1, 2);
    piece2 = getDominoPieceWith(6, 1);
    EXPECT_THAT(piece.isApplicableTo(piece2), Eq(true));
}

TEST_F(GameTest, a1And2Is_Not_ApplicableToA6And6) {
    DominoPiece piece = getDominoPieceWith(1, 2);
    DominoPiece piece2 = getDominoPieceWith(6, 6);
    EXPECT_THAT(piece.isApplicableTo(piece2), Eq(false));
}

TEST_F(GameTest, aNormalPieceHas2MountPoints) {
    DominoPiece piece = getDominoPieceWith(1, 2);
    PlayGround playGround = PlayGround(piece);

    EXPECT_THAT(playGround.getAvailableMountPoints().size(), Eq(2));
}

TEST_F(GameTest, aStoneHasAMountPoint) {
    DominoPiece piece = getDominoPieceWith(1, 2);
    PlayGround playGround = PlayGround(piece);

    EXPECT_THAT(playGround.getAvailableMountPoints().size(), Eq(1));
}

TEST_F(GameTest, twoStonesHave2MountPoints) {
    DominoPiece piece = getDominoPieceWith(1, 2);
    DominoPiece piece2 = getDominoPieceWith(6, 6);

    PlayGround playGround = PlayGround(piece);
    EXPECT_THAT(playGround.getAvailableMountPoints().size(), Eq(1));

    DominoTreeStructureElement nextMountPoint = playGround.getAvailableMountPoints().front();
    playGround.mountStone(nextMountPoint, piece2);
    DominoTreeStructureElement rootElement = playGround.getRootElement();
    EXPECT_THAT(playGround.getAvailableMountPoints().size(), Eq(2));
}


