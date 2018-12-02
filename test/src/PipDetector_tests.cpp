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
    VS files = {"../aHalf2Img.jpg", "../bHalf2Img.jpg"};
    VI expectedPips = {2, 2};

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

TEST_F(PipsDetectorTest, testAHalfWith4Pips) {
    verify(0);
}

TEST_F(PipsDetectorTest, testAHalfWithNoPips) {
    verify(1);
}

