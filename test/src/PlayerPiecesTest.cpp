//
// Created by osboxes on 12/13/18.
//


#include <opencv2/core/mat.hpp>
#include <I_ImageHandler.h>
#include <ImageHandlerFactory.h>
#include <PipsDetector.h>
#include <DominoCV.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;


class PlayerPiecesTest : public ::testing::Test {

protected:
    I_ImageHandler* imageHandler;
    
    virtual void SetUp() {
        imageHandler = ImageHandlerFactory::getImageHandler("../../srcImgWithPlayerStones", "field_", Source::FILESYSTEM);
    };

    virtual void TearDown() {
    };

    virtual void verify(int index) {
        
        cv::Mat previousImg = cv::Mat();
        cv::Mat currentImg = cv::Mat();
        do {
            imageHandler->loadNextImage();
            currentImg = imageHandler->getCurrentImage();
            if (NULL == &currentImg || currentImg.empty()) {
                std::cout << "No more Image loaded" << std::endl;

                return;// EXIT_SUCCESS;
            }

            previousImg = imageHandler->getPreviousImage();

        } while (previousImg.empty());

        const vector<dominoPiece> &pieces = detectPlayerDominoPieces(imageHandler->getFirstImage(), currentImg);
        for(auto dominoPiece : pieces) {
            std::cout << "found piece " << dominoPiece.a.pips << "," << dominoPiece.b.pips << std::endl;
        }

        EXPECT_EQ(pieces.size(), 5);
    }
};


// The test should return 0 pieces without crashing
TEST_F(PlayerPiecesTest, testImageWithNoPieces) {
    cv::Mat firstImgMat = cv::imread("../gestell_0.jpg");
    if (!firstImgMat.data) {
        FAIL() << "Could not open or find the image";
    }

    cv::Mat previousImgMat = cv::imread("../gestell_1.jpg");
    if (!previousImgMat.data) {
        FAIL() << "Could not open or find the image";
    }

    cv::Mat currentImgMat = cv::imread("../gestell_2.jpg");
    if (!currentImgMat.data) {
        FAIL() << "Could not open or find the image";
    }

    //detectPiece(previousImgMat, currentImgMat);

    const vector<dominoPiece> &pieces = detectPlayerDominoPieces(firstImgMat, currentImgMat);
    ASSERT_THAT(pieces.size(), testing::Eq(0));
}

// The test should return 0 pieces without crashing
TEST_F(PlayerPiecesTest, testImageWithNoPieces2) {
    cv::Mat firstImgMat = cv::imread("../gestell_0.jpg");
    if (!firstImgMat.data) {
        FAIL() << "Could not open or find the image";
    }

    cv::Mat previousImgMat = cv::imread("../gestell_2.jpg");
    if (!previousImgMat.data) {
        FAIL() << "Could not open or find the image";
    }

    cv::Mat currentImgMat = cv::imread("../gestell_3.jpg");
    if (!currentImgMat.data) {
        FAIL() << "Could not open or find the image";
    }

    //detectPiece(previousImgMat, currentImgMat);

    const vector<dominoPiece> &pieces = detectPlayerDominoPieces(firstImgMat, currentImgMat);
    ASSERT_THAT(pieces.size(), testing::Eq(0));
}

TEST_F(PlayerPiecesTest, testRecognitionOfPlayerDominoPieces) {
    verify(0);
}

