//
// Created by osboxes on 05.12.18.
//

#ifndef PROJECT_FILEIMAGEMANAGER_H
#define PROJECT_FILEIMAGEMANAGER_H



//Interface
#include "I_ImageHandler.h"

class FileImageHandler : public I_ImageHandler {
public:
    FileImageHandler(std::string path, std::string imagePrefix);

    cv::Mat getCurrentImage();
    cv::Mat getPreviousImage();
    void loadNextImage();
    void setPath(std::string path, std::string imagePrefix);
    ~FileImageHandler();

private:
    std::string pathPostfix = "";
    std::string imagePostfix = ".jpg";
    std::string sourcePath = "";
    std::string targetPath ="";
    unsigned int numberOfRecievedImages = 0;

    bool testFileExistence(const std::string& name);
};


#endif //PROJECT_FILEIMAGEMANAGER_H
