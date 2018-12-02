// std lib
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

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
   // PipsDetector *pipsdetector = PipsDetectorFactory().createDefaultPipsDetector();


    /***************************************************************************
     * load the Picture with new Domino and the predecessor picture
     */
    //new Domino
    cv::Mat img = cv::imread("../../img/gestell_1.jpg");
    if (!img.data) {
        cout << "Could not open or find the new image" << std::endl;
        return -1;
    }

    //predeccessors
    cv::Mat imgPrevious = cv::imread("../../img/gestell_0.jpg");
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
    cv::absdiff(frame, imgPrevious,frame); 
    diffframe = frame.clone();
    cv::imwrite("domino_diff.jpg", frame);

    // threshold - converting to binary image
    cv::threshold(frame, frame,120, 255, cv::THRESH_BINARY );
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
    const float EPS = 0.001;
    bool isCorrectionOfAngleNeeded = ((getCorrectedAngle(minAreaRotatedRect) -  minAreaRotatedRect.angle) > EPS) ? (true): (false);
    std::cout << "isCorrectionOfAngleNeeded: " <<  isCorrectionOfAngleNeeded <<std::endl;

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

    //set random corner as startCorner
    cv::Point2f startCornerHalf1 = cornerPoints[0];
    cv::Point2f startCornerHalf2 = cornerPoints[2];

    std::thread t1(getDominoHalf, &half1, diffframe.clone(), cornerPoints, startCornerHalf1, isCorrectionOfAngleNeeded);
    std::thread t2(getDominoHalf, &half2, diffframe.clone(), cornerPoints, startCornerHalf2, isCorrectionOfAngleNeeded);
    t1.join();
    t2.join();

    std::cout << "pipcount half 1: " << half1.pips << std::endl;
    std::cout << "pipcount half 2: " << half2.pips << std::endl;

    cv::Point2f half1CornerPoints[4];
    cv::Point2f half2CornerPoints[4];
    half1.rect.points(half1CornerPoints);
    half2.rect.points(half2CornerPoints);
    std::string numberCharactermapping[4] = {"A","B","C","D"};
    for(int i = 0; i < 4 ; i++){
        cv::circle(unprocessedFrame, half1CornerPoints[i], 3, brightColor, 1, 8, 0);
        cv::circle(unprocessedFrame, half2CornerPoints[i], 3, brightColor, 1, 8, 0);
        putText(unprocessedFrame, "1"+numberCharactermapping[i],  half1CornerPoints[i],  0, 1, brightColor,1, 8,false );
        putText(unprocessedFrame, "2"+numberCharactermapping[i],  half2CornerPoints[i],  0, 1, brightColor, 1, 8, false );
    }


    unprocessedFrame = drawPipCount(half1, unprocessedFrame);
    unprocessedFrame = drawPipCount(half2, unprocessedFrame);

    unprocessedFrame = colorizeHalf(half1, unprocessedFrame);
    unprocessedFrame = colorizeHalf(half2, unprocessedFrame);


    printer->printImage("frame", unprocessedFrame);
    cv::imwrite("domino_result.jpg", unprocessedFrame);


}
