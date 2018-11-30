// std lib
#include <iostream>
#include <sstream>

#include "ImgDebugPrinter/ImgDebugPrinter.h"
#include "PipsDetector/PipsDetector.h"
#include "DominoLib/DominoLib.h"

// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

// color for drawing into img
cv::Scalar brightColor = cv::Scalar(255, 0, 242);

struct dominoHalf {
    cv::RotatedRect rect;
    unsigned int pips;
};

struct dominoPiece {
    dominoHalf a;
    dominoHalf b;
};

int main(int argc, char **argv) {

    cout << "Running main" << std::endl;

    PipsDetector *pipsdetector = PipsDetectorFactory().createDefaultPipsDetector();
    AbstractImgDebugPrinter* printer = IImgDebugPrinterFactory().getPrinter();

    cv::Mat img = cv::imread("../../img/tisch_6_rot.jpg");

    if (!img.data) {
        cout << "Could not open or find the new image" << std::endl;
        return -1;
    }

    cv::Mat imgPrevious = cv::imread("../../img/tisch_5_rot.jpg");

    if (!imgPrevious.data) {
        cout << "Could not open or find the previous image" << std::endl;
        return -1;
    }

    cvtColor(imgPrevious, imgPrevious, CV_BGR2GRAY);

    // will hold the frame of the origin image for output with additional information
    cv::Mat unprocessedFrame = img.clone();
    // will hold our frame to do image processing on
    cv::Mat frame = img;
    // will hold the difference of the current and the previous frame
    cv::Mat diffframe;

    // convert to grayscale
    cvtColor(frame, frame, CV_BGR2GRAY);

    // remove background
    cv::absdiff(frame, imgPrevious, frame);
    cv::imwrite("domino_diff.jpg", frame);
    diffframe = frame.clone();

    // threshold
    cv::threshold(frame, frame, 150, 255, cv::THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite("domino_bin.jpg", frame);

    // applying blur filter
    cv::blur( frame.clone(), frame, cv::Size(3,3) );
    cv::imwrite("domino_bin_blur.jpg", frame);

    // applying canny edge filter
    cv::Canny(frame, frame, 2, 2 * 2, 3, false);
    cv::imwrite("domino_canny.jpg", frame);

    cv::imwrite("frame.jpg", frame);

    // detect domino piece shape
    std::vector<std::vector<cv::Point> > pieceContours;
    std::vector<cv::Vec4i> diceHierarchy;
    cv::findContours(frame.clone(), pieceContours, diceHierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat drawing = cv::Mat::zeros(img.size(), CV_8UC3);
    for(unsigned int i=0; i < pieceContours.size(); i++) {
        cv::Scalar color = cv::Scalar(150, 0, 32);
        drawContours(drawing, pieceContours, i, brightColor, 2, 8);

        cv::Mat singleContour = cv::Mat::zeros(img.size(), CV_8UC3);
        drawContours(singleContour, pieceContours, i, brightColor, 2, 8);
        std::ostringstream contourName;
        contourName << "domino_contour" <<  i << ".jpg";
        cv::imwrite( contourName.str(), singleContour);
    }

    cv::imwrite("domino_contours.jpg", drawing);

    // iterate over dice contours
    for(int i=0; i < pieceContours.size(); i++){
        auto diceContour = pieceContours.at(i);

        cv::RotatedRect minAreaRotatedRect = cv::minAreaRect(diceContour);
        cv::Mat rotated = unprocessedFrame.clone();
        drawRotatedRect(rotated, minAreaRotatedRect);

        cv::imwrite("domino_rotated_rect.jpg", rotated);

        cv::Rect diceBoundsRect = minAreaRotatedRect.boundingRect();

        // get contour area
        double diceContourArea = diceBoundsRect.area();

        std::cout << "area: " << diceContourArea << std::endl;

        // filter contours based on our dice size
        if (diceContourArea > DOMINO_PIECE_AREA_MIN && diceContourArea < DOMINO_PIECE_AREA_MAX) {

            // set dice roi
            cv::Mat diceROI = diffframe(diceBoundsRect);

            cv::imwrite("rect.jpg", diceROI);

            dominoHalf halfA;
            dominoHalf halfB;
            cv::Point2f cornerPoints[4];
            minAreaRotatedRect.points(cornerPoints);
            cv::circle(unprocessedFrame, cornerPoints[0], 2, brightColor);
            cv::circle(unprocessedFrame, cornerPoints[1], 2, brightColor);
            cv::circle(unprocessedFrame, cornerPoints[2], 2, brightColor);
            cv::circle(unprocessedFrame, cornerPoints[3], 2, brightColor);
            cv::imwrite("domino_cornerPoints.jpg", unprocessedFrame);

            cv::Point2f point1HalfA = cornerPoints[0];
            cv::Point2f point2HalfA;
            cv::Point2f point3HalfA;
            cv::Point2f point1HalfB;
            cv::Point2f point2HalfB;

            std::vector<cv::Point2f> cornerPointsVector;
            cornerPointsVector.insert( cornerPointsVector.begin(), cornerPoints, cornerPoints + 4);
            std::sort(cornerPointsVector.begin(), cornerPointsVector.end(), [point1HalfA](cv::Point2f const& a, cv::Point2f const& b) {
                return cv::norm(a-point1HalfA) < cv::norm(b-point1HalfA);
            });
            assert(point1HalfA == cornerPointsVector.at(0));
            point2HalfA = cornerPointsVector.at(1);
            point3HalfA = (cornerPointsVector.at(2) - point1HalfA) / 2 + point1HalfA;
            point1HalfB = cornerPointsVector.at(2);
            point2HalfB = cornerPointsVector.at(3);

            cv::putText(unprocessedFrame, "A", point1HalfA, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
            cv::putText(unprocessedFrame, "B", point2HalfA, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
            cv::putText(unprocessedFrame, "C", point3HalfA, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);

            halfA.rect = cv::RotatedRect(point2HalfA, point1HalfA, point3HalfA);
            halfB.rect = cv::RotatedRect(point2HalfB, point1HalfB, point3HalfA);

            cv::Rect halfARect = halfA.rect.boundingRect();
            cv::Mat halfARotatedROI = diffframe(halfARect);
            cv::Mat halfAUpright;
            rotate2D(halfARotatedROI, halfAUpright, halfA.rect.angle);
            float orgCols = static_cast<float>(halfARotatedROI.cols);
            float targetCols = static_cast<float>(halfAUpright.cols);
            float scaleX = static_cast<float>(halfAUpright.rows) / halfARotatedROI.rows ;
            float scaleY = static_cast<float>(halfAUpright.cols) / halfARotatedROI.cols ;

            std::cout << "scale y: " << scaleY << std::endl ;

            cv::Point2f offsetOfOrigin = cv::Point2f(halfARect.x, halfARect.y);
            cv::Point2f rotatedC = RotatePoint(halfA.rect.center, point3HalfA,  3.141592653 + (halfA.rect.angle * 3.141592653)) - offsetOfOrigin;
            //rotatedC = cv::Point2f(rotatedC.x * scaleX, rotatedC.y * scaleY);
            cv::Point2f rotatedA = RotatePoint(halfA.rect.center, point1HalfA, 3.141592653 +(halfA.rect.angle * 3.141592653)) - offsetOfOrigin;
            //rotatedA = cv::Point2f(rotatedA.x * scaleX, rotatedA.y * scaleY);
            cv::Point2f rotatedB = RotatePoint(halfA.rect.center, point2HalfA, 3.141592653 + (halfA.rect.angle * 3.141592653)) - offsetOfOrigin;
            //rotatedB = cv::Point2f(rotatedB.x * scaleX, rotatedB.y * scaleY);

            /********************************************
             * calc D
             */
            //first: get Center of rectangle
            // difference vector CB (diagonal) - half it ->Vector AM
             /*
            cv::Point2f CB = cv::Point2f((rotatedB  - rotatedC));
            std::cout << "rotatedC | x: " << rotatedC.x << " y: " << rotatedC.y << std::endl;
            std::cout << "rotatedB | x: " << rotatedB.x << " y: " << rotatedB.y << std::endl;
            std::cout << "CB | x: " << CB.x << " y: " << CB.y << std::endl;
            cv::Point2f M = cv::Point2f(CB/2 + rotatedC);
            std::cout << "rotatedC | x: " << rotatedC.x << " y: " << rotatedC.y << std::endl;
            std::cout << "M | x: " << M.x << " y: " << M.y << std::endl;
            cv::Point2f AM = cv::Point(rotatedA-M);
            cv::Point2f AM2 = cv::Point2f(AM*2);
            cv::Point2f rotatedD = cv::Point2f(rotatedA + AM2);*/

             cv::Point2f rotatedD = rotatedC + (rotatedB-rotatedA);
            cv::Point2f M =(rotatedA + rotatedB  + rotatedC+rotatedD)/4;
            //cv::Point2f rotatedD = RotatePoint(halfA.rect.center, cv::Point2f(point3HalfA.x, point2HalfA.y),3.141592653 + (halfA.rect.angle * 3.141592653)) - offsetOfOrigin;
            //rotatedD = cv::Point2f(rotatedD.x * scaleX, rotatedD.y * scaleY);
            cv::Point2f zero = cv::Point2f(0,0);
            std::vector<cv::Point2f> cornerVector{rotatedC,rotatedA, rotatedB, rotatedD};
            std::sort(cornerVector.begin(), cornerVector.end(), [zero](cv::Point2f const& a, cv::Point2f const& b) {
                return cv::norm(a-zero) < cv::norm(b-zero);
            });

            cv::Point2f topLeft = cornerVector.at(0);
            cv::Point2f bottomRight = cornerVector.at(3);
            std::cout<< "topLeft: " << topLeft.x << " , " << topLeft.y << std::endl;
            const cv::Rect halfAUprightROI = cv::Rect(topLeft, bottomRight);

            cv::Point2f newCenter = cv::Point2f(halfAUpright.cols/2, halfAUpright.rows/2);
            cv::Point2f correctionOfCenter = newCenter - M;
            rotatedA = rotatedA + correctionOfCenter;
            rotatedB = rotatedB + correctionOfCenter;
            rotatedC = rotatedC + correctionOfCenter;
            rotatedD = rotatedD + correctionOfCenter;
            M = M + correctionOfCenter;



            cv::putText(halfAUpright, "A", rotatedA, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
            cv::putText(halfAUpright, "B", rotatedB, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
            cv::putText(halfAUpright, "C", rotatedC, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
            cv::putText(halfAUpright, "D", rotatedD, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
            cv::putText(halfAUpright, "M", M, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
            cv::line(halfAUpright,rotatedB, rotatedA, brightColor, 1);
            cv::line(halfAUpright,rotatedB, rotatedC, brightColor, 1);
            cv::line(halfAUpright,rotatedB, rotatedD, brightColor, 1);
            cv::line(halfAUpright,rotatedA, rotatedC, brightColor, 1);
            cv::line(halfAUpright,rotatedC, rotatedD, brightColor, 1);
            cv::line(halfAUpright,rotatedA, rotatedD, brightColor, 1);
           // cv::rectangle(halfAUpright, halfAUprightROI, brightColor, CV_FILLED, 8, 0);


            cv::Point2f topLeftCorner = cv::Point2f(0, 0);
            cv::Point2f topRightCorner = cv::Point2f( halfAUpright.cols,0);
            cv::Point2f bottomLeftCorner = cv::Point2f(0, halfAUpright.rows);
            cv::Point2f bottomRightCorner = cv::Point2f(halfAUpright.cols, halfAUpright.rows);

            cv::line(halfAUpright,rotatedA, rotatedD, brightColor, 1);

            cv::line(halfAUpright,newCenter, topLeftCorner, brightColor, 1);
            cv::line(halfAUpright,newCenter, topRightCorner, brightColor, 1);
            cv::line(halfAUpright,newCenter, bottomLeftCorner, brightColor, 1);
            cv::line(halfAUpright,newCenter, bottomRightCorner, brightColor, 1);

            cv::putText(halfAUpright, "Z", newCenter, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
            cv::imwrite("domino_halfAUprightROI.jpg", halfAUpright);

            //halfA.pips = numberOfPipsA;
            //halfB.pips = numberOfPipsB;

            //drawRotatedRect(unprocessedFrame, halfA.rect);

            std::cout << halfA.pips << std::endl;
            drawRotatedRect(unprocessedFrame, halfA.rect);
            std::cout << halfB.pips << std::endl;

            //assert(halfA.rect.angle == halfB.rect.angle);

            cv::imwrite("domino_halfARect.jpg", unprocessedFrame);
            //a.rect = cv::RotatedRect(minAreaRotatedRect.)

            // rotate img to zero degree
            cv::Point2f center = cvPoint(diceBoundsRect.height / 2, diceBoundsRect.width / 2);

            cv::Mat upright;

            /*#### Correct rotation angle - Set Upright (+90deg) - Set Horizontal (90deg - angle)### */
            float corrected_angle_deg = minAreaRotatedRect.angle;
            if (minAreaRotatedRect.size.width > minAreaRotatedRect.size.height) {
                corrected_angle_deg = 90 + corrected_angle_deg;
            }
            rotate2D(diceROI, upright, corrected_angle_deg);

            //cv::line(upright, cv::Point(0, upright.rows/2), cv::Point(upright.cols, upright.rows/2), brightColor, 1);
            cv::imwrite("domino_upright.jpg", upright);

            cv::Rect aHalfRect = cv::Rect( cv::Point(0,0), cv::Point(upright.cols, upright.rows/2));
            //cv::rectangle(aHalf, aHalfRect, brightColor, CV_FILLED, 8, 0);
            cv::Mat aHalfImg = upright(aHalfRect);
            cv::imwrite("aHalfImg.jpg", aHalfImg);

            cv::Rect bHalfRect = cv::Rect( cv::Point(0,upright.rows/2), cv::Point(upright.cols, upright.rows));
            //cv::rectangle(aHalf, aHalfRect, brightColor, CV_FILLED, 8, 0);
            cv::Mat bHalfImg = upright(bHalfRect);
            cv::imwrite("bHalfImg.jpg", bHalfImg);

            std::ostringstream pipsResults;
            pipsResults << "Number of pips a,b: ";
            unsigned int numberOfPipsA = pipsdetector->detect(aHalfImg);
            unsigned int numberOfPipsB = pipsdetector->detect(bHalfImg);
            pipsResults << numberOfPipsA << "," << numberOfPipsB;
            std::cout << pipsResults.str() << std::endl;

            unsigned int numberOfAllPips = numberOfPipsA + numberOfPipsB;
            if (numberOfAllPips > 0) {

                // output debug info
                std::ostringstream diceText;
                diceText << "val: " << numberOfPipsA << " + " << numberOfPipsB << " = " <<numberOfAllPips;

                std::cout << diceText.str() << std::endl;

                // draw value
                cv::putText(unprocessedFrame, diceText.str(),
                            cv::Point(diceBoundsRect.x, diceBoundsRect.y + diceBoundsRect.height + 20),
                            cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8
                );

                // draw bounding rect
                //cv::rectangle(unprocessedFrame, diceBoundsRect.tl(), diceBoundsRect.br(), brightColor, 2, 8, 0);
                cv::drawContours(unprocessedFrame, pieceContours, 0, brightColor, 2, 8, diceHierarchy);

                // show
                printer->printImage("frame", unprocessedFrame);
                cv::imwrite("domino_result.jpg", unprocessedFrame);
            } else {
                std::cout << "No pips found!" << std::endl;
            }
        }
    }
}
