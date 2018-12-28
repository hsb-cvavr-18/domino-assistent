//
// Created by osboxes on 04.12.18.
//

#ifndef PROJECT_FILESYSTEMHANDLER_H
#define PROJECT_FILESYSTEMHANDLER_H


//Interface
#include "I_ImageHandler.h"

class IpcamHandler : public I_ImageHandler {
public:
    //constructor
    IpcamHandler(std::string path, std::string imagePrefix);

    // Implement functions
    cv::Mat getFirstImage() override;
    cv::Mat getCurrentImage() override;
    cv::Mat getPreviousImage() override;
    void loadNextImage() override;
    void setPath(std::string path, std::string imagePrefix);

    ~IpcamHandler();

private:
    std::string pathPostfix = "http://";
    std::string imagePostfix = ".jpg";
    std::string sourcePath = "";
    std::string targetPath ="";
    unsigned int numberOfRecievedImages = 0;

};


#endif //PROJECT_FILESYSTEMHANDLER_H
