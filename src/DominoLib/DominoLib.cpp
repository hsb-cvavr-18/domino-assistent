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

float getCorectedAngle(cv::RotatedRect rotRect){
    /*#### Correct rotation angle - Set Upright (+90deg) - Set Horizontal (90deg - angle)### */
    float corrected_angle_deg = rotRect.angle;
    if (rotRect.size.width > rotRect.size.height) {
        corrected_angle_deg = 90 + corrected_angle_deg;
    }
    return corrected_angle_deg;
}

cv::RotatedRect getRotatedRectOflargestContour(std::vector<std::vector<cv::Point> > pieceContours){
    float area = 0;
    cv::RotatedRect largestRotatedRect;
    for(unsigned int i=0; i < pieceContours.size(); i++) {
        auto diceContour = pieceContours.at(i);
        //get rotatedRect framing contour
        cv::RotatedRect minAreaRotatedRect = cv::minAreaRect(diceContour);
        // get rotatedRect framed contour area
        float diceContourArea = minAreaRotatedRect.size.area();

        std::cout << "area: " << diceContourArea << std::endl;
        //find largest rotated rect
        if(diceContourArea > area){
            largestRotatedRect  = minAreaRotatedRect;
        }
    }
    return largestRotatedRect;
}

cv::Mat getROIOfHalf(cv::Mat diffframe, cv::Point2f cornerA, cv::Point2f cornerB, cv::Point2f cornerC, cv::Point2f cornerD){
    cv::RotatedRect half = cv::RotatedRect(cornerA, cornerB, cornerC); //anticlockwise

    cv::Rect halfBoundingRect = half.boundingRect();
    cv::Mat halfRotatedROI = diffframe(halfBoundingRect);
    cv::Mat halfCorrectedRot;
    rotate2D(halfRotatedROI, halfCorrectedRot, half.angle);


    //new rotated frame has new dimensions and a new origin => offset for correction
    cv::Point2f halfOffset= cv::Point2f(halfBoundingRect.x, halfBoundingRect.y);

    // correction of corners in new frame
    cv::Point2f rotatedA = (RotatePoint(half.center, cornerA, PI + ((half.angle + 0) * PI/180)) - halfOffset);
    cv::Point2f rotatedB = (RotatePoint(half.center, cornerB, PI + ((half.angle + 0) * PI/180)) - halfOffset);
    cv::Point2f rotatedC = (RotatePoint(half.center, cornerC, PI + ((half.angle + 0) * PI/180)) - halfOffset);
    cv::Point2f rotatedD = (RotatePoint(half.center, cornerD, PI + ((half.angle + 0) * PI/180)) - halfOffset);

    //Move it to hte correct center.
    cv::Point2f halfM = ((rotatedA + rotatedB  + rotatedC + rotatedD) / 4) ; //Middle of half1CorrectedRot
    cv::Point2f halfCenter = cv::Point2f(halfCorrectedRot.cols/2, halfCorrectedRot.rows/2); //Center of half1
    cv::Point2f correctionOfCenter = halfCenter - halfM;
    halfM = halfM + correctionOfCenter; //corrected center
    rotatedA = rotatedA + correctionOfCenter;
    rotatedB = rotatedB + correctionOfCenter;
    rotatedC = rotatedC + correctionOfCenter;
    rotatedD = rotatedD + correctionOfCenter;

    cv::Rect halfRect = cv::Rect(rotatedA, rotatedC);
    //cv::Mat half1ROI=
    return halfCorrectedRot(halfRect);
}
