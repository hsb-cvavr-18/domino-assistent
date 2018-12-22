//
// Created by osboxes on 12/13/18.
//


#include <opencv2/core/mat.hpp>
#include "gtest/gtest.h"
#include <I_ImageHandler.h>
#include <ImageHandlerFactory.h>
#include <PipsDetector.h>
#include <DominoCV.h>

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

TEST_F(PlayerPiecesTest, testRecognitionOfPlayerDominoPieces) {
    verify(0);
}

