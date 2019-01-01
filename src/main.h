//
// Created by Konstantin Gredeskoul on 10/7/18.
//

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H


// std lib
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#include "DominoLib/DominoLib.h"
#include "ImgDebugPrinter/ImgDebugPrinter.h"
#include "PipsDetector/PipsDetector.h"
#include "ImageHandler/I_ImageHandler.h"
#include "ImageHandler/ImageHandlerFactory.h"
// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>

#include "Game/PlayGround.h"
#include "DominoLib/DominoCV.h"
#include "ImageClipping/ImageClippingFactory.h"
#include "ImageClipping/ImageClipping.h"
#include "DominoLib/SafeQueue.h"

void waitForUserInput() {
    string cinBuff;
    std::cout << "Enter key to take next img" << std::endl;
    std::getline(std::cin, cinBuff);
}

#endif //PROJECT_MAIN_H
