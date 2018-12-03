//
// Created by osboxes on 12/3/18.
//

//
// Created by osboxes on 11/12/18.
//

#include <opencv2/core/mat.hpp>
#include "gtest/gtest.h"
#include <DominoCV.h>

using namespace std;

#define VS vector<string>
#define VI vector<long long>

class DominoCVTest : public ::testing::Test {

protected:
    virtual void SetUp() {
    };

    virtual void TearDown() {
    };

    virtual void verify(string previousImg, string currentImg, int pipsAExpected, int pipsBExpected) {

        cv::Mat previousImgMat = cv::imread(previousImg);
        if (!previousImgMat.data) {
            FAIL() << "Could not open or find the image '" << previousImg << "'";
        }

        cv::Mat currentImgMat = cv::imread(currentImg);
        if (!currentImgMat.data) {
            FAIL() << "Could not open or find the image '" << currentImg << "'";
        }

        const dominoPiece &dominoPiece = detectPieces(previousImgMat, currentImgMat);

        EXPECT_EQ(dominoPiece.a.pips, pipsAExpected);
        EXPECT_EQ(dominoPiece.b.pips, pipsBExpected);
    }
};

TEST_F(DominoCVTest, gestell0To1) {
    verify("../gestell_0.jpg", "../gestell_1.jpg", 2, 6);
}

TEST_F(DominoCVTest, gestell6To7) {
    verify("../gestell_6.jpg", "../gestell_7.jpg", 1, 5);
}

TEST_F(DominoCVTest, gestell7To8) {
    verify("../gestell_7.jpg", "../gestell_8.jpg", 1, 6);
}

TEST_F(DominoCVTest, gestell8To9) {
    verify("../gestell_8.jpg", "../gestell_9.jpg", 6, 6);
}

TEST_F(DominoCVTest, gestell9To10) {
    verify("../gestell_9.jpg", "../gestell_10.jpg", 0, 6);
}

TEST_F(DominoCVTest, gestell10To11) {
    verify("../gestell_10.jpg", "../gestell_11.jpg", 1, 2);
}

TEST_F(DominoCVTest, gestell11To12) {
    verify("../gestell_11.jpg", "../gestell_12.jpg", 1, 1);
}

TEST_F(DominoCVTest, gestell12To13) {
    verify("../gestell_12.jpg", "../gestell_13.jpg", 3, 1);
}
