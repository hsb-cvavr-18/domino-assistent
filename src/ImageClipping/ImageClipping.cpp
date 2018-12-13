//
// Created by osboxes on 06.12.18.
//

#include "ImageClipping.h"

ImageClipping::ImageClipping(PlayerPosition playerPosition, float size, float padding){
    this->playerPosition = playerPosition;
}

ImageClipping::ImageClipping(const cv::Mat  sourceImage, PlayerPosition playerPosition, float size, float padding){
    assert(size>= 0 && size <=100);
    assert(padding >=0 && padding <=100);
    this->playerPosition = playerPosition;
    this->playersAreaSize = size/100;
    this->padding = padding/100;
    if(!sourceImage.empty()) {
        this->sourceImage = sourceImage;
    }
    this->calcAreas();

}

ImageClipping::~ImageClipping(){
}

void ImageClipping::setSourceImage(const cv::Mat  sourceImage){
    if(sourceImage.empty()) {
        std::cout
                << "ERROR: Could not set Image. Illegal Argument in ImageClipping::setSourceImage(cv::Mat sourceImage)"
                << std::endl;
        return;
    }
    this->sourceImage = sourceImage;
    //std::cout << "this source image: " << &this->sourceImage << ", source image:" << &sourceImage << std::endl;
    if(playersArea.empty() || fieldArea.empty())
        this->calcAreas();
}

cv::Mat ImageClipping::getOverlayedImage(){
    double alpha = 0.3;
    this->overlayedImage = this->sourceImage.clone(); // cv::Mat::zeros(this->sourceImage.rows,this->sourceImage.cols,this->sourceImage.type()); 	//this->sourceImage.clone();
    //cv::Mat sourceCopy = this->sourceImage.clone();

  //  cv::Rect test = cv::Rect(25,25,100,100);
    //cv::rectangle(sourceCopy, test,cv::Scalar(0,255,0), 1, 8, 0 );

    cv::rectangle(this->overlayedImage, this->fieldArea,FIELD_COLOR,  CV_FILLED, 8, 0 );
    cv::rectangle(this->overlayedImage, this->playersArea,PLAYER_COLOR,  CV_FILLED, 8, 0 );
    cv::addWeighted(this->overlayedImage,alpha, this->sourceImage, 1- alpha, 0, overlayedImage);
    for(int i = 0; i < NUMBER_OF_PLAYER_BLOCKS; i ++){
        cv::rectangle(overlayedImage, this->blockAreas[i], BLOCK_COLOR, 8, 8, 0 );
    }

    return overlayedImage;
}

cv::Mat ImageClipping::getPlayersAreaImage(){

    this->playerImage = this->sourceImage(this->playersArea);
    return this->playerImage;
}

cv::Mat ImageClipping::getPlayingFieldImage(){
    this->fieldImage = this->sourceImage(this->fieldArea);
    return this->fieldImage;
}


cv::Rect ImageClipping::calcPlayBlockArea(int blockNumber){
    assert(blockNumber >=0 && blockNumber <=6);
    float size;
    cv::Point2f topLeftStart;

    switch ( this->playerPosition )
    {
        case PlayerPosition::POS_BOTTOM:
            size = (this->sourceImage.cols - this->sourceImage.cols * 2 * this->padding) / (NUMBER_OF_PLAYER_BLOCKS);
            //std::cout << "max: " << this->sourceImage.cols << std::endl;
            //std::cout << "size: " << size << std::endl;
            topLeftStart = cv::Point2f(round(this->sourceImage.cols  * this->padding + blockNumber * size), round(this->sourceImage.rows - this->sourceImage.rows * this->playersAreaSize));
            this->blockAreas[blockNumber] = cv::Rect (topLeftStart.x, topLeftStart.y, size, this->sourceImage.rows * this->playersAreaSize);
            break;
        case PlayerPosition::POS_TOP:
            size = (this->sourceImage.cols - this->sourceImage.cols * 2 * this->padding) / (NUMBER_OF_PLAYER_BLOCKS);
            //std::cout << "max: " << this->sourceImage.cols << std::endl;
            //std::cout << "size: " << size << std::endl;
            topLeftStart = cv::Point2f(round(this->sourceImage.cols *  this->padding + blockNumber * size), 0);
            this->blockAreas[blockNumber] = cv::Rect (topLeftStart.x, topLeftStart.y, size, this->sourceImage.rows * this->playersAreaSize);
            break;
        case PlayerPosition::POS_RIGHT:
            size = (this->sourceImage.rows - this->sourceImage.rows * 2 * this->padding)/ (NUMBER_OF_PLAYER_BLOCKS);
            topLeftStart = cv::Point2f(round(this->sourceImage.cols - this->sourceImage.cols * this->playersAreaSize), round(this->sourceImage.rows *  this->padding + blockNumber * size));
            this->blockAreas[blockNumber]  = cv::Rect (topLeftStart.x, topLeftStart.y, this->sourceImage.cols * this->playersAreaSize, size);
            break;
        case PlayerPosition::POS_LEFT:
            size = (this->sourceImage.rows - this->sourceImage.rows * 2 * this->padding)/ (NUMBER_OF_PLAYER_BLOCKS);
            topLeftStart = cv::Point2f(0, round(this->sourceImage.rows * this->padding + blockNumber * size));
            this->blockAreas[blockNumber]  = cv::Rect (topLeftStart.x, topLeftStart.y, this->sourceImage.cols * this->playersAreaSize, size);
            break;
        default:
            break;
    }

}

void ImageClipping::calcAreas(){

    cv::Point playersTopLeftCorner;
    cv::Point playersBottomRightCorner;
    cv::Point fieldTopLeftCorner;
    cv::Point fieldBottomRightCorner;

    switch ( this->playerPosition )
    {
        case PlayerPosition::POS_TOP:
            playersTopLeftCorner = cv::Point2f(0, 0);
            playersBottomRightCorner = cv::Point2f(this->sourceImage.cols, round(this->sourceImage.rows * playersAreaSize));
            fieldTopLeftCorner = cv::Point2f(0,round(this->sourceImage.rows * playersAreaSize ));
            fieldBottomRightCorner = cv::Point2f(this->sourceImage.cols, this->sourceImage.rows);

            break;
        case PlayerPosition::POS_RIGHT:
            playersTopLeftCorner = cv::Point2f(round((this->sourceImage.cols) - (this->sourceImage.cols)  * playersAreaSize), 0);
            playersBottomRightCorner = cv::Point2f(this->sourceImage.cols, this->sourceImage.rows);
            fieldTopLeftCorner = cv::Point2f(0,0);
            fieldBottomRightCorner = cv::Point2f(round((this->sourceImage.cols) - (this->sourceImage.cols)  * playersAreaSize), this->sourceImage.rows);
            break;
        case PlayerPosition::POS_BOTTOM:
            playersTopLeftCorner = cv::Point2f(0, round(this->sourceImage.rows - this->sourceImage.rows * playersAreaSize));
            playersBottomRightCorner = cv::Point2f(this->sourceImage.cols, this->sourceImage.rows);
            fieldTopLeftCorner = cv::Point2f(0,0);
            fieldBottomRightCorner = cv::Point2f(this->sourceImage.cols, round(this->sourceImage.rows - this->sourceImage.rows * playersAreaSize));

            break;
        case PlayerPosition::POS_LEFT:
            playersTopLeftCorner = cv::Point2f(0, 0);
            playersBottomRightCorner = cv::Point2f(round(this->sourceImage.cols  * playersAreaSize), this->sourceImage.rows);
            fieldTopLeftCorner = cv::Point2f(round(this->sourceImage.cols  * playersAreaSize),0);
            fieldBottomRightCorner = cv::Point2f(this->sourceImage.cols,this->sourceImage.rows);

            break;
        default:
            break;
    }

    this->playersArea = cv::Rect(playersTopLeftCorner,playersBottomRightCorner);
    this->fieldArea = cv::Rect(fieldTopLeftCorner,fieldBottomRightCorner);

    for(int i = 0; i < NUMBER_OF_PLAYER_BLOCKS; i++){
        this->calcPlayBlockArea(i);
    }


    return;

}


cv::Rect ImageClipping::getPlayerDominiBlock(int blockNumber){
    assert(blockNumber >=0 && blockNumber <=NUMBER_OF_PLAYER_BLOCKS);
    return this->blockAreas[blockNumber];
}