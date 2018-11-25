//
// Created by osboxes on 11/25/18.
//

#ifndef CMAKE_DOMINOLIB_H
#define CMAKE_DOMINOLIB_H

#include <iostream>

using namespace std;

void drawRotatedRect(cv::Mat &image, cv::RotatedRect rotatedRect);
void rotate2D(const cv::Mat &src, cv::Mat &dst, const double degrees);

#endif //CMAKE_DOMINOLIB_H
