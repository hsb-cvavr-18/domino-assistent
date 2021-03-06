//
// Created by osboxes on 06.12.18.
//

#ifndef PROJECT_IMAGECLIPPING_H
#define PROJECT_IMAGECLIPPING_H

//std
#include <stdio.h>
#include <iostream>

//opencv includes
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>


struct RectOffset {
    cv::Rect area;
    cv::Point offset;
} ;

struct MatOffset {
    cv::Mat roi;
    cv::Point offset;
} ;



enum class PlayerPosition {POS_TOP, POS_RIGHT, POS_BOTTOM, POS_LEFT};
const float PLAYERS_AREA_SIZE_PERCENT = 0.15; //*100 = percent
const cv::Scalar PLAYER_COLOR = cv::Scalar(255, 0, 0);
const cv::Scalar FIELD_COLOR = cv::Scalar(0, 255, 0);
const cv::Scalar BLOCK_COLOR = cv::Scalar(0, 0, 0);
const int NUMBER_OF_PLAYER_BLOCKS = 7;


class ImageClipping {

public:
    ImageClipping(PlayerPosition playerPosition, float size = 15, float padding = 12.5);
    ImageClipping(const cv::Mat sourceImage, PlayerPosition playerPosition = PlayerPosition::POS_RIGHT, float size = 15, float padding = 12);
    ~ImageClipping();
    void setSourceImage(const cv::Mat sourceImage);
    //cv::Mat getOverlayedImage();
    cv::Mat getOverlayedImage();
    //cv::Mat getPlayersAreaImage();
    MatOffset getPlayersAreaImage();
    //cv::Mat getPlayingFieldImage();
    MatOffset getPlayingFieldImage();
    RectOffset getPlayerDominiBlock(int blockNumber);


private:
    cv::Mat sourceImage;    //full image
    cv::Mat overlayedImage;
    MatOffset fieldImage; //area where player mount matching domino blocks
    MatOffset playerImage; //area where player places their own domino blocks
    PlayerPosition playerPosition; // Postion where the area for the players domino blocks will be
    //cv::Rect playersArea;
    RectOffset playersArea;
    //cv::Rect fieldArea;
    RectOffset fieldArea;
    //cv::Rect blockAreas[NUMBER_OF_PLAYER_BLOCKS];
    RectOffset blockAreas[NUMBER_OF_PLAYER_BLOCKS];
    float padding = 0.125;
    float playersAreaSize = 0.15; //*100 => percent
    void calcAreas();
    RectOffset calcPlayBlockArea(int blockNumber);
    //cv::Rect calcPlayBlockArea(int blockNumber);

};


#endif //PROJECT_IMAGECLIPPING_H
