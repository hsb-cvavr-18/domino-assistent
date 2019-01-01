//
// Created by osboxes on 11/25/18.
//

#ifndef CMAKE_DOMINOLIB_H
#define CMAKE_DOMINOLIB_H

#include "../ImgDebugPrinter/ImgDebugPrinter.h"
#include "../PipsDetector/PipsDetector.h"

// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include "../Game/DominoHalf.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#define PI 3.14159265358979323846
using namespace std;


void drawRotatedRect(cv::Mat &image, cv::RotatedRect rotatedRect);
void rotate2D(const cv::Mat &src, cv::Mat &dst, const double degrees);
cv::Point2f RotatePoint(const cv::Point2f &p, float rad);
cv::Point2f RotatePoint(const cv::Point2f &cen_pt, const cv::Point2f &p, float rad);
float getCorrectedAngle(cv::RotatedRect rotRect);
cv::RotatedRect getRotatedRectOflargestContour(vector<vector<cv::Point>> pieceContours, cv::Size size_);
cv::Mat getROIOfHalf(cv::Mat diffframe, cv::Point2f cornerA, cv::Point2f cornerB, cv::Point2f cornerC, cv::Point2f cornerD, bool correctAngle);
cv::Mat colorizeHalf(DominoHalf half, cv::Mat  img);
cv::Mat drawPipCount(DominoHalf half1, cv::Mat  img);
void printTopAreasAndContours(const vector<vector<cv::Point>> &pieceContours, cv::Size &size_);

void getHalfCorners(cv::Point2f *cornersOfDominoBlock, cv::Point2f startCorner, cv::Point2f *targetCorners);
void getDominoHalf(DominoHalf *half, cv::Mat diffframe, cv::Point2f *cornersOfDominoBlock, cv::Point2f startCorner,  bool correctAngle);

#endif //CMAKE_DOMINOLIB_H
