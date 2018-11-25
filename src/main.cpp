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


int main(int argc, char **argv) {

    cout << "Running main" << std::endl;

    PipsDetector *pipsdetector = PipsDetectorFactory().createDefaultPipsDetector();
    AbstractImgDebugPrinter* printer = IImgDebugPrinterFactory().getPrinter();

    cv::Mat img = cv::imread("../../img/tisch_5_rot.jpg");

    if (!img.data) {
        cout << "Could not open or find the new image" << std::endl;
        return -1;
    }

    cv::Mat imgPrevious = cv::imread("../../img/tisch_4_rot.jpg");

    if (!imgPrevious.data) {
        cout << "Could not open or find the previous image" << std::endl;
        return -1;
    }

    cvtColor(imgPrevious, imgPrevious, CV_BGR2GRAY);

    // will hold our frame
    cv::Mat frame;
    cv::Mat unprocessedFrame;
    cv::Mat diffframe;

    // color for drawing into img
    cv::Scalar brightColor = cv::Scalar(255, 0, 242);

    frame = img;

    // hold unprocessed framed
    unprocessedFrame = frame.clone();

    // convert to grayscale
    cvtColor(frame, frame, CV_BGR2GRAY);

    // remove background
    cv::absdiff(frame, imgPrevious, frame);
    cv::imwrite("domino_diff.jpg", frame);
    diffframe = frame.clone();

    // threshold
    cv::threshold(frame, frame, 150, 255, cv::THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite("domino_bin.jpg", frame);

    cv::Mat tmp = frame.clone();
    cv::blur( tmp, frame, cv::Size(3,3) );
    cv::imwrite("domino_bin_blur.jpg", frame);

    // applying canny edge filter
    cv::Canny(frame, frame, 2, 2 * 2, 3, false);
    cv::imwrite("domino_canny.jpg", frame);

    cv::imwrite("frame.jpg", frame);

    // detect dice shapes
    std::vector<std::vector<cv::Point> > diceContours;
    std::vector<cv::Vec4i> diceHierarchy;
    cv::findContours(frame.clone(), diceContours, diceHierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat drawing = cv::Mat::zeros(img.size(), CV_8UC3);
    for(int i=0; i < diceContours.size(); i++) {
        cv::Scalar color = cv::Scalar(150, 0, 32);
        drawContours(drawing, diceContours, i, color, 2, 8, diceHierarchy, 0, cv::Point());

        cv::Mat singleContour = cv::Mat::zeros(img.size(), CV_8UC3);
        drawContours(singleContour, diceContours, i, color, 2, 8, diceHierarchy, 0, cv::Point());
        std::ostringstream contourName;
        contourName << "domino_contour" <<  i << ".jpg";
        cv::imwrite( contourName.str(), singleContour);
    }
    cv::imwrite("domino_contours.jpg", drawing);

    // iterate over dice contours
    for(int i=0; i < diceContours.size(); i++){
        auto diceContour = diceContours.at(i);

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
            long numberOfPipsA = pipsdetector->detect(aHalfImg);
            long numberOfPipsB = pipsdetector->detect(bHalfImg);
            pipsResults << numberOfPipsA << "," << numberOfPipsB;
            std::cout << pipsResults.str() << std::endl;

            long numberOfAllPips = numberOfPipsA + numberOfPipsB;
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
                cv::drawContours(unprocessedFrame, diceContours, 0, brightColor, 2, 8, diceHierarchy);

                // show
                printer->printImage("frame", unprocessedFrame);
                cv::imwrite("domino_result.jpg", unprocessedFrame);
            }
            else {
                std::cout << "No pips found!" << std::endl;
            }
        }
    }
}
