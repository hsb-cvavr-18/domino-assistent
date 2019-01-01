//
// Created by osboxes on 11/25/18.
//

// std lib
#include <iostream>
#include <sstream>
#include <algorithm>

//includes

#include "DominoLib.h"

cv::Scalar overlayColors[8] = {cv::Scalar(244, 66, 66),  //color 0
                               cv::Scalar(244, 66, 244), //color 1
                               cv::Scalar(89, 66, 244),  //color 2
                               cv::Scalar(66, 244, 244), //color 3
                               cv::Scalar(66, 244, 122), //color 4
                               cv::Scalar(122, 244, 66), //color 5
                               cv::Scalar(244, 122, 66), //color 6
                               cv::Scalar(255, 0, 0)};//ERROR COLOR

// color for drawing into img
cv::Scalar drawingColor = cv::Scalar(255, 0, 242);

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

float getCorrectedAngle(cv::RotatedRect rotRect){
    /*#### Correct rotation angle - Set Upright (+90deg) - Set Horizontal (90deg - angle)### */
    float corrected_angle_deg = rotRect.angle;
    if (rotRect.size.width > rotRect.size.height) {
        corrected_angle_deg = 90 + corrected_angle_deg;
    }
    return corrected_angle_deg;
}

float getMinRectAreaOfContour(const std::vector<cv::Point> &pieceContour) {
    cv::RotatedRect minAreaRotatedRect = cv::minAreaRect(pieceContour);
    // get rotatedRect framed contour area
    float area = minAreaRotatedRect.size.area();
    return area;
}

cv::RotatedRect getRotatedRectOflargestContour(vector<vector<cv::Point>> pieceContours, cv::Size imgSize) {
    std::sort(pieceContours.begin(), pieceContours.end(),
              [](std::vector<cv::Point> const &a, std::vector<cv::Point> const &b) {
                  return getMinRectAreaOfContour(a) > getMinRectAreaOfContour(b);
              });

    cv::RotatedRect largestRotatedRect  = cv::minAreaRect(pieceContours.at(0));

    printTopAreasAndContours(pieceContours, imgSize);

    return largestRotatedRect;
}

void printTopAreasAndContours(const vector<vector<cv::Point>> &pieceContours, cv::Size &imgSize) {
    cout << "Top 3 areas in image: "  << endl;
    cv::Mat pieceContour = cv::Mat::zeros(imgSize, CV_8UC3);
    for(int i = 0; i < pieceContours.size() && i < 3; i++) {
        const auto &slice = pieceContours.at(i);
        cout << getMinRectAreaOfContour(slice) << endl;
        cv::Scalar color = cv::Scalar(150, 0, 32);
        cv::drawContours(pieceContour, pieceContours, 0, color, 2, 8);
    }
    cv::imwrite("largest3_contours.jpg", pieceContour);
}

cv::Mat getROIOfHalf(cv::Mat diffframe, cv::Point2f cornerA, cv::Point2f cornerB, cv::Point2f cornerC, cv::Point2f cornerD, bool correctAngle){


    cv::RotatedRect half = cv::RotatedRect(cornerA, cornerB, cornerC); //anticlockwise

    cv::Rect halfBoundingRect = half.boundingRect();
    cv::Mat halfRotatedROI = diffframe(halfBoundingRect);
    cv::Mat halfCorrectedRot;
    rotate2D(halfRotatedROI, halfCorrectedRot, half.angle);

    float scaleX = 1; // (float)halfCorrectedRot.cols / halfBoundingRect.width;
    float scaleY = 1; // (float)halfCorrectedRot.rows / halfBoundingRect.height;

    cornerA = cv::Point2f (cornerA.x * scaleX, cornerA.y * scaleY);
    cornerB = cv::Point2f (cornerB.x * scaleX, cornerB.y * scaleY);
    cornerC = cv::Point2f (cornerC.x * scaleX, cornerC.y * scaleY);
    cornerD = cv::Point2f (cornerD.x * scaleX, cornerD.y * scaleY);
    //new rotated frame has new dimensions and a new origin => offset for correction
    cv::Point2f halfOffset= cv::Point2f(halfBoundingRect.x * scaleX, halfBoundingRect.y * scaleY);

    cv::Point2f rotatedA;
    cv::Point2f rotatedB;
    cv::Point2f rotatedC;
    cv::Point2f rotatedD;

    // correction of corners in new frame
    if(correctAngle) {
        rotatedA = (RotatePoint(half.center, cornerA, PI - ((half.angle + 0) * PI / 180)) - halfOffset);
        rotatedB = (RotatePoint(half.center, cornerB, PI - ((half.angle + 0) * PI / 180)) - halfOffset);
        rotatedC = (RotatePoint(half.center, cornerC, PI - ((half.angle + 0) * PI / 180)) - halfOffset);
        rotatedD = (RotatePoint(half.center, cornerD, PI - ((half.angle + 0) * PI / 180)) - halfOffset);
    } else {
        rotatedA = (RotatePoint(half.center, cornerA, PI + ((half.angle + 0) * PI / 180)) - halfOffset);
        rotatedB = (RotatePoint(half.center, cornerB, PI + ((half.angle + 0) * PI / 180)) - halfOffset);
        rotatedC = (RotatePoint(half.center, cornerC, PI + ((half.angle + 0) * PI / 180)) - halfOffset);
        rotatedD = (RotatePoint(half.center, cornerD, PI + ((half.angle + 0) * PI / 180)) - halfOffset);
    }


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

    return halfCorrectedRot(halfRect);
}

cv::Mat colorizeHalf(DominoHalf half, cv::Mat  img){
    cv::Point2f vertices2f[4];
    half.getRect().points(vertices2f);
    cv::Scalar halfColor;
    halfColor = (half.getNumber() >= 0 && half.getNumber() < 7) ? (overlayColors[half.getNumber()]) : (overlayColors[sizeof(overlayColors)/sizeof(overlayColors[0])-1]);

    cv::Point vertices[4];
    for(int i = 0; i < 4; i++){
        vertices[i]=vertices2f[i];
    }
    cv::Mat overlay = img.clone();
    double alpha = 0.3;
    cv::fillConvexPoly(overlay, vertices,4,halfColor);
    cv::addWeighted(overlay,alpha, img, 1- alpha, 0, img );
    return img;
}

cv::Mat drawSuggestedMove(DominoPiece userStone, DominoPiece recommendedStone,cv::Mat img){
    cv::arrowedLine(img,userStone.getCenter(),recommendedStone.getCenter(), cv::Scalar(33, 244, 33),3);
    return img;
}

cv::Mat drawPipCount(DominoHalf half1, cv::Mat  img){
    std::ostringstream pipCountText;
    pipCountText << half1.getNumber();
    cv::putText(img, pipCountText.str(),
                cv::Point(half1.getRect().center.x, half1.getRect().center.y),
                cv::FONT_HERSHEY_COMPLEX, 0.8, drawingColor, 1, 8
    );
    return img;
}


void getHalfCorners(cv::Point2f *cornersOfDominoBlock, cv::Point2f startCorner, cv::Point2f *targetCorners){
    //declare Corners of Half 1
    cv::Point2f corners[4];

    //find corner point next known to half1CornerA (short side of domino block)
    std::vector<cv::Point2f> dominoCornersVector;
    dominoCornersVector.insert(dominoCornersVector.begin(), cornersOfDominoBlock, cornersOfDominoBlock + 4);
    std::sort(dominoCornersVector.begin(), dominoCornersVector.end(),
              [startCorner](cv::Point2f const &a, cv::Point2f const &b) {
                  return cv::norm(a -  startCorner) < cv::norm(b -  startCorner);
              });

    //outer corners
    targetCorners[0] = dominoCornersVector.at(0);    //1A
    targetCorners[3] = dominoCornersVector.at(1);    //1D

    //bisecting corners
    targetCorners[1] =  targetCorners[0] + ((dominoCornersVector.at(2) - targetCorners[0])/2);    //1A + ((2D-1A)/2)
    targetCorners[2] =  targetCorners[3] + ((dominoCornersVector.at(3) - targetCorners[3])/2);    //1D + ((2A-1D)/2)
}


void getDominoHalf(DominoHalf *half, cv::Mat diffframe, cv::Point2f *cornersOfDominoBlock, cv::Point2f startCorner,  bool correctAngle){
    cv::Point2f halfCorners[4];
    PipsDetector *pipsdetector = PipsDetectorFactory().createDefaultPipsDetector();

    getHalfCorners(cornersOfDominoBlock, startCorner, halfCorners);
    std::cout << halfCorners[0].x << " "<< halfCorners[1].x << " "<< halfCorners[2].x << " "<< halfCorners[3].x << std::endl;
    //dominoHalf half;
    //get rectangles framing each of the two halfs
    half->setRect(cv::RotatedRect(halfCorners[0], halfCorners[1], halfCorners[2])); //anticlockwise
    //get region Of Interest of Half
    cv::Mat halfROI = getROIOfHalf(diffframe, halfCorners[0], halfCorners[1], halfCorners[2], halfCorners[3], correctAngle);
    //Get Pips of  half of the domino block
    half->setNumber(pipsdetector->detect(halfROI));
}

