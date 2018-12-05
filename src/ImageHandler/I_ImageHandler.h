//
// Created by osboxes on 04.12.18.
//

#ifndef PROJECT_IMAGEHANDLER_H
#define PROJECT_IMAGEHANDLER_H

//std
#include <stdio.h>
#include <iostream>
#include <sys/stat.h>

//opencv includes
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>


class I_ImageHandler {
public:
    // pure virtual function
    virtual cv::Mat getCurrentImage() = 0;
    virtual cv::Mat getPreviousImage() =  0;
    virtual void loadNextImage() = 0;
    virtual void setPath(std::string path, std::string imagePrefix) = 0;
    virtual ~I_ImageHandler(){};

protected:
    I_ImageHandler(){};
    cv::Mat currentImage;
    cv::Mat previousImage;
    std::string workingImagePath = "images/";
};


#endif //PROJECT_IMAGEHANDLER_H
