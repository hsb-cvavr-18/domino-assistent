//
// Created by osboxes on 12/3/18.
//

#ifndef PROJECT_DOMINOCV_H
#define PROJECT_DOMINOCV_H

// std lib
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <future>

#include "../DominoLib/DominoLib.h"
#include "../ImgDebugPrinter/ImgDebugPrinter.h"
#include "../PipsDetector/PipsDetector.h"
#include "../ImageClipping/ImageClippingFactory.h"
#include "../ImageClipping/ImageClipping.h"
// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include "../Game/DominoPiece.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>


DominoPiece detectPiece(cv::Mat previousImg, cv::Mat currentImg);
vector<DominoPiece> detectPlayerDominoPieces(cv::Mat firstImg, cv::Mat currentImg, ImageClipping *imageClipper);
DominoPiece getPlayerDominoPiece(ImageClipping *imageClipper, cv::Mat firstImg, cv::Mat currentImg, int blockNumber);
cv::Mat cutPlayerBlock(cv::Mat image, cv::Rect rect);
DominoPiece detectPiece(cv::Mat previousImg, cv::Mat currentImg);

#endif //PROJECT_DOMINOCV_H
