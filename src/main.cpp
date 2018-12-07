
// std lib
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#include "DominoLib/DominoLib.h"
#include "ImgDebugPrinter/ImgDebugPrinter.h"
#include "PipsDetector/PipsDetector.h"
#include "ImageHandler/I_ImageHandler.h"
#include "ImageHandler/ImageHandlerFactory.h"
// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>

#include "DominoLib/DominoCV.h"

#include "ImageClipping/ImageClipping.h"
void task_main();
void task_preview(std::string address);
int main(int argc, char **argv) {
    cout << "Running main" << std::endl;



    thread t2(task_main);
    task_preview("192.168.43.152");
    t2.join();



    return EXIT_SUCCESS;
}


void task_main(){
    /***************************************************************************
   * load the Picture with new Domino and the predecessor picture
   */
    //new Domino
    auto imageHandler = ImageHandlerFactory::getImageHandler("../../srcImg", "gestell_", Source::FILESYSTEM);
    //auto imageHandler = ImageHandlerFactory::getImageHandler("192.168.178.79:8080", "photo", Source::IP_CAM);
    cv::Mat currentImg = cv::Mat();
    cv::Mat previousImg = cv::Mat();
    ImageClipping *imageClipper = new ImageClipping(PlayerPosition::POS_LEFT, 15,12.5);

    while(true) {
        //TODO: Verarbeitung der Bilder (Logik - wann wird ausgelöst, behandlung der ersten zwei Bilder etc.
        do {
            imageHandler->loadNextImage();
            currentImg = imageHandler->getCurrentImage();
            if (NULL == &currentImg || currentImg.empty()) {
                std::cout << "No more Image loaded" << std::endl;

                return;// EXIT_SUCCESS;
            }

            previousImg = imageHandler->getPreviousImage();

        } while (previousImg.empty());


        imageClipper->setSourceImage(currentImg);
        cv::Mat playerImg = imageClipper->getPlayersAreaImage();
        cv::Mat playingFieldMarked = imageClipper->getOverlayedImage();

        namedWindow( "PlayingAreas", cv::WINDOW_NORMAL );// Create a window for display.
        cv::resizeWindow("PlayingAreas", playingFieldMarked.cols / 2.5, playingFieldMarked.rows / 2.5);
        imshow( "PlayingAreas", playingFieldMarked );
        cv::waitKey();

        const dominoPiece &dominoPiece = detectPiece(previousImg, currentImg);

        cout << "pipcount half 1: " << dominoPiece.a.pips << endl;
        cout << "pipcount half 2: " << dominoPiece.b.pips << endl;
    }

}



void task_preview(std::string address)
{
    cv::VideoCapture vcap;
    cv::Mat image;
    float aspectRatio;
    int width = 500;
    const std::string videoStreamAddress = "http://" + address + ":8080/video";

    //open the video stream and make sure it's opened
    if(!vcap.open(videoStreamAddress)) {
        std::cout << "Error opening video stream or file" << std::endl;
        return;
    }


    std::cout << "bup" << endl;
    namedWindow( "Preview", cv::WINDOW_NORMAL );// Create a window for display
    cv::resizeWindow("Preview", width, width);
    cv::moveWindow("Preview", 1920 - (width +50),20);

    for(;;) {
        if(!vcap.read(image)) {
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }


        imshow( "Preview", image );
        if(cv::waitKey(1) >= 0) break;
    }
}

