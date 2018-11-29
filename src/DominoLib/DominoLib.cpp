//
// Created by osboxes on 11/25/18.
//

// std lib
#include <iostream>
#include <sstream>

#include "DominoLib.h"


// https://stackoverflow.com/questions/43342199/draw-rotated-rectangle-in-opencv-c
void drawRotatedRect(cv::Mat &image, cv::RotatedRect rotatedRect) {
    cv::Point centerPoint = rotatedRect.center;
    cv::Size rectangleSize = rotatedRect.size;
    double rotationDegrees = rotatedRect.angle;

    cv::Scalar color = cv::Scalar(255.0, 255.0, 255.0); // white

    // Create the rotated rectangle
    cv::RotatedRect rotatedRectangle(centerPoint, rectangleSize, rotationDegrees);

    // We take the edges that OpenCV calculated for us
    cv::Point2f vertices2f[4];
    rotatedRectangle.points(vertices2f);

    // Convert them so we can use them in a fillConvexPoly
    cv::Point vertices[4];
    for (int i = 0; i < 4; ++i) {
        vertices[i] = vertices2f[i];

    }

    // Now we can fill the rotated rectangle with our specified color
    cv::fillConvexPoly(image, vertices, 4, color);

}

void rotate2D(const cv::Mat &src, cv::Mat &dst, const double degrees) {
    cv::Point2f center(src.cols / 2.0, src.rows / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, degrees, 1.0);
    cv::Rect bbox = cv::RotatedRect(center, src.size(), degrees).boundingRect();

    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

    cv::warpAffine(src, dst, rot, bbox.size());
}

cv::Point2f RotatePoint(const cv::Point2f &p, float rad) {
    const float x = std::cos(rad) * p.x - std::sin(rad) * p.y;
    const float y = std::sin(rad) * p.x + std::cos(rad) * p.y;

    const cv::Point2f rot_p(x, y);
    return rot_p;
}

cv::Point2f RotatePoint(const cv::Point2f &cen_pt, const cv::Point2f &p, float rad) {
    const cv::Point2f trans_pt = p - cen_pt;
    const cv::Point2f rot_pt = RotatePoint(trans_pt, rad);
    const cv::Point2f fin_pt = rot_pt + cen_pt;

    return fin_pt;
}