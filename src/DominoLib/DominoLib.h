//
// Created by osboxes on 11/25/18.
//

#ifndef CMAKE_DOMINOLIB_H
#define CMAKE_DOMINOLIB_H

// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

void drawRotatedRect(cv::Mat &image, cv::RotatedRect rotatedRect);
void rotate2D(const cv::Mat &src, cv::Mat &dst, const double degrees);
cv::Point2f RotatePoint(const cv::Point2f &p, float rad);
cv::Point2f RotatePoint(const cv::Point2f &cen_pt, const cv::Point2f &p, float rad);

#endif //CMAKE_DOMINOLIB_H
