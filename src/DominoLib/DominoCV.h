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

#include "../DominoLib/DominoLib.h"
#include "../ImgDebugPrinter/ImgDebugPrinter.h"
#include "../PipsDetector/PipsDetector.h"
// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>


dominoPiece detectPieces(cv::Mat previousImg, cv::Mat currentImg);

#endif //PROJECT_DOMINOCV_H
