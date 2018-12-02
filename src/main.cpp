// std lib
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>

#include "DominoLib/DominoLib.h"
#include "ImgDebugPrinter/ImgDebugPrinter.h"
#include "PipsDetector/PipsDetector.h"
// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>


// color for drawing into img
cv::Scalar brightColor = cv::Scalar(255, 0, 242);
cv::VideoCapture vcap;
cv::Mat image;

int main(int argc, char **argv) {

    cout << "Running main" << std::endl;
/*
    const std::string videoStreamAddress = "http://192.168.178.79:8080/video";

    //open the video stream and make sure it's opened
    if(!vcap.open(videoStreamAddress)) {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    std::cout << "Load next Image" << std::endl;
    std::cin.get();
    if(!vcap.read(image)) {
        std::cout << "No frame" << std::endl;
        cv::waitKey();
    }

    cv::imshow("Output Window", image);

    while (true) {
        FILE* file = popen("wget -q http://192.168.178.79:8080/photoaf.jpg -O latest_img.jpg","r");
        fclose(file);

        cv::Mat image = cv::imread("latest_img.jpg");
        cv::namedWindow( "Image output" );
        cv::imshow("Image output", image); cv::waitKey(5); // here's your car ;)
    }*/
    AbstractImgDebugPrinter *printer = IImgDebugPrinterFactory().getPrinter();
    PipsDetector *pipsdetector = PipsDetectorFactory().createDefaultPipsDetector();


    /***************************************************************************
     * load the Picture with new Domino and the predecessor picture
     */
    //new Domino
    cv::Mat img = cv::imread("../../img/tisch_5_rot.jpg");
    if (!img.data) {
        cout << "Could not open or find the new image" << std::endl;
        return -1;
    }

    //predeccessors
    cv::Mat imgPrevious = cv::imread("../../img/tisch_4_rot.jpg");
    if (!imgPrevious.data) {
        cout << "Could not open or find the previous image" << std::endl;
        return -1;
    }

    // will hold the frame of the origin image for output with additional information
    cv::Mat unprocessedFrame = img.clone();
    // will hold our frame to do image processing on
    cv::Mat frame = img;
    // will hold the difference of the current and the previous frame
    cv::Mat diffframe;


    /***************************************************************************
     * Seperating new domino block and reduce date for further processing
     */
    // convert to grayscale
    cvtColor(frame, frame, CV_BGR2GRAY);
    cvtColor(imgPrevious, imgPrevious, CV_BGR2GRAY);

    // remove background (previous image -image)
    cv::absdiff(frame, imgPrevious,
                frame); //TODO: why not "cv::absdiff(frame, imgPrevious, diffframe);" ? Would save one op
    diffframe = frame.clone();
    cv::imwrite("domino_diff.jpg", frame);

    // threshold - converting to binary image
    cv::threshold(frame, frame, 150, 255, cv::THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite("domino_bin.jpg", frame);

    // applying blur filter - makes edges smoother an closes gaps for continuous edges
    cv::blur(frame.clone(), frame, cv::Size(3, 3));
    cv::imwrite("domino_bin_blur.jpg", frame);

    // applying canny edge filter
    cv::Canny(frame, frame, 2, 2 * 2, 3, false);
    cv::imwrite("domino_canny.jpg", frame);

    cv::imwrite("frame.jpg", frame);


    /***************************************************************************
     * Find new Domino block in image
     */
    //find all first level contours (CV_RETR_EXTERNAL)
    std::vector<std::vector<cv::Point> > pieceContours;
    std::vector<cv::Vec4i> diceHierarchy;
    cv::findContours(frame.clone(), pieceContours, diceHierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    //Find the largest contour and create a rotated rect around it
    cv::RotatedRect minAreaRotatedRect = getRotatedRectOflargestContour(pieceContours);

    //need the angle to be corrected ?
    bool correctAngle = getCorectedAngle(minAreaRotatedRect) -  minAreaRotatedRect.angle ;
    std::cout << "correctAngle: " <<  correctAngle <<std::endl;

    //get bounding box of rotated rect
    cv::Rect dominoBoundsRect = minAreaRotatedRect.boundingRect();

    //Just for the show (debugging)
    cv::Mat rotated = unprocessedFrame.clone();
    drawRotatedRect(rotated, minAreaRotatedRect);
    cv::imwrite("domino_rotated_rect.jpg", rotated);

    //Cut out wanted domino - containeing only wanted Domino
    cv::Mat diceROI = diffframe(dominoBoundsRect);
    cv::imwrite("rect.jpg", diceROI);


    /***************************************************************************
     * Get Pips of each half of the domino block
     */
    /***************************************************************************
     * Find and define halfs
     */
    dominoHalf half1;
    dominoHalf half2;

    //get corners of whole block
    cv::Point2f cornerPoints[4];
    minAreaRotatedRect.points(cornerPoints);

    //find corners of both halfs
    //declare Corners of Half 2
    cv::Point2f half1CornerA, half1CornerB,half1CornerC,half1CornerD ;

    //declare Corners of Half 2
    cv::Point2f half2CornerA, half2CornerB, half2CornerC, half2CornerD;

    //set random corner as corner A
    half1CornerA = cornerPoints[0];

    //find corner point next known to half1CornerA (short side of domino block)
    std::vector<cv::Point2f> dominoCornersVector;
    dominoCornersVector.insert(dominoCornersVector.begin(), cornerPoints, cornerPoints + 4);
    std::sort(dominoCornersVector.begin(), dominoCornersVector.end(),
              [half1CornerA](cv::Point2f const &a, cv::Point2f const &b) {
                  return cv::norm(a - half1CornerA) < cv::norm(b - half1CornerA);
              });

    //outer corners
    //half 1
    half1CornerA = dominoCornersVector.at(0);
    half1CornerD = dominoCornersVector.at(1);
    //half 2
    half2CornerA = dominoCornersVector.at(3);
    half2CornerD = dominoCornersVector.at(2);

    //bisecting corners
    half1CornerB = half1CornerA + ((half2CornerD - half1CornerA) / 2);
    half1CornerC = half1CornerD + ((half2CornerA - half1CornerD) / 2);

    half2CornerB = half1CornerC;
    half2CornerC = half1CornerB;

    //Just for the show  (debugging)
    cv::putText(unprocessedFrame, "1A", half1CornerA, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
    cv::putText(unprocessedFrame, "1B", half1CornerB, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
    cv::putText(unprocessedFrame, "1C", half1CornerC, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
    cv::putText(unprocessedFrame, "1D", half1CornerD, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
    cv::putText(unprocessedFrame, "2A", half2CornerA, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);
    cv::putText(unprocessedFrame, "2D", half2CornerD, cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8);

    cv::circle(unprocessedFrame, half1CornerA, 2, brightColor);
    cv::circle(unprocessedFrame, half1CornerB, 2, brightColor);
    cv::circle(unprocessedFrame, half1CornerC, 2, brightColor);
    cv::circle(unprocessedFrame, half1CornerD, 2, brightColor);
    cv::circle(unprocessedFrame, half2CornerA, 2, brightColor);
    cv::circle(unprocessedFrame, half2CornerB, 2, brightColor);
    cv::circle(unprocessedFrame, half2CornerC, 2, brightColor);
    cv::circle(unprocessedFrame, half2CornerD, 2, brightColor);
    cv::imwrite("domino_cornerPoints.jpg", unprocessedFrame);

    //Launch a thread

    std::thread t1(getDominoHalf,diffframe.clone(), &half1, pipsdetector, half1CornerA, half1CornerB, half1CornerC, half1CornerD, correctAngle);
    std::thread t2(getDominoHalf,diffframe.clone(), &half2, pipsdetector, half2CornerA, half2CornerB, half2CornerC, half2CornerD, correctAngle);
    t1.join();
    t2.join();

    std::cout <<  half1.pips  << std::endl;
    std::cout <<  half2.pips  << std::endl;

    //getDominoHalf(diffframe.clone(), half1, pipsdetector, half1CornerA, half1CornerB, half1CornerC, half1CornerD, correctAngle);
    //getDominoHalf(diffframe.clone(), half2, pipsdetector, half2CornerA, half2CornerB, half2CornerC, half2CornerD, correctAngle);
    /*
    //get rectangles framing each of the two halfs
    half1.rect = cv::RotatedRect(half1CornerA, half1CornerB, half1CornerC); //anticlockwise
    half2.rect = cv::RotatedRect(half2CornerA, half2CornerB, half2CornerC); //anticlockwise


    cv::Mat half1ROI = getROIOfHalf(diffframe, half1CornerA, half1CornerB, half1CornerC, half1CornerD, correctAngle);
    cv::Mat half2ROI = getROIOfHalf(diffframe, half2CornerA, half2CornerB, half2CornerC, half2CornerD, correctAngle);

    cv::imwrite("domino_half2ROI.jpg", half2ROI);
    cv::imwrite("domino_half1ROI.jpg", half1ROI);
    */
    /***************************************************************************
    *  Get Pips of each half of the domino block
    */
    /*
    half1.pips = pipsdetector->detect(half1ROI);
    half2.pips = pipsdetector->detect(half2ROI);

    std::cout << "numberOfPips1 " << half1.pips << std::endl;
    std::cout << "numberOfPips2 " << half2.pips << std::endl;
    */


    unprocessedFrame = drawPipCount(half1, unprocessedFrame);
    unprocessedFrame =drawPipCount(half2, unprocessedFrame);

    unprocessedFrame = colorizeHalf(half1, unprocessedFrame);
    unprocessedFrame = colorizeHalf(half2, unprocessedFrame);


    printer->printImage("frame", unprocessedFrame);
    cv::imwrite("domino_result.jpg", unprocessedFrame);


}
