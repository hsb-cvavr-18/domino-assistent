//
// Created by osboxes on 11/12/18.
//

#include <opencv2/core/mat.hpp>
#include "gtest/gtest.h"
#include <PipsDetector.h>

using namespace std;

#define VS vector<string>
#define VI vector<long long>

class PipsDetectorTest : public ::testing::Test {

protected:
    VS files = {"../aHalf0Img.jpg", "../aHalf1Img.jpg", "../aHalf2Img.jpg", "../aHalf2Img_2.jpg", "../aHalf4Img.jpg"};
    VI expectedPips = {0, 1, 2, 2, 4};

    virtual void SetUp() {
    };

    virtual void TearDown() {
    };

    virtual void verify(int index) {
        PipsDetector *pipsdetector = PipsDetectorFactory().createDefaultPipsDetector();
        basic_string<char> &filename = files.at(index);
        cv::Mat img = cv::imread(filename, CV_8UC1);

        if (!img.data) {
            FAIL() << "Could not open or find the image '" << filename << "'";
        }

        long pipsExpected = expectedPips.at(index);

        long pipsActual = pipsdetector->detect(img);

        EXPECT_EQ(pipsActual, pipsExpected);
    }
};

TEST_F(PipsDetectorTest, testAHalfWithNoPip) {
    verify(0);
}

TEST_F(PipsDetectorTest, testAHalfWithOnePip) {
    verify(1);
}

TEST_F(PipsDetectorTest, testAHalfWith2Pips) {
    verify(2);
}

TEST_F(PipsDetectorTest, testAHalfWith2Pips2) {
    verify(3);
}

TEST_F(PipsDetectorTest, testAHalfWith4Pips) {
    verify(4);
}

