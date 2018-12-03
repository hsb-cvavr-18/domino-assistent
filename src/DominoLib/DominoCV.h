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
#include <type_traits>

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

using perf_clock = std::conditional<
        std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock
>::type;

using floating_seconds = std::chrono::duration<double>;

struct measure
{
    template<typename F, typename ...Args>
    static typename std::chrono::duration<double> execution(F func, Args&&... args)
    {
        const auto t0 = perf_clock::now();
        std::forward<F>(func)(std::forward<Args>(args)...);
        return std::chrono::duration<double>(perf_clock::now() - t0);
    }
};

dominoPiece detectPieceInternal(dominoPiece &piece, cv::Mat previousImg, cv::Mat currentImg);
dominoPiece detectPiece(cv::Mat previousImg, cv::Mat currentImg);

#endif //PROJECT_DOMINOCV_H
