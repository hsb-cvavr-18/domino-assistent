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
#include "DominoLib/DominoCV.h"

int main(int argc, char **argv) {/*
    cv::VideoCapture vcap;
    cv::Mat image;

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


    cout << "Running main" << std::endl;

    /***************************************************************************
    * load the Picture with new Domino and the predecessor picture
    */
    //new Domino
    cv::Mat currentImg = cv::imread("../../img/gestell_8.jpg");
    if (!currentImg.data) {
        cout << "Could not open or find the new image" << endl;
        exit(EXIT_FAILURE);
    }

    //predeccessors
    cv::Mat previousImg = cv::imread("../../img/gestell_7.jpg");
    if (!previousImg.data) {
        cout << "Could not open or find the previous image" << endl;
        exit(EXIT_FAILURE);
    }

    const dominoPiece &dominoPiece = detectPiece(previousImg, currentImg);

    cout << "pipcount half 1: " << dominoPiece.a.pips << endl;
    cout << "pipcount half 2: " << dominoPiece.b.pips << endl;
    
    return EXIT_SUCCESS;
}

