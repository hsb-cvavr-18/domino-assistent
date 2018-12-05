//
// Created by osboxes on 04.12.18.
//

#include "IpcamHandler.h"

IpcamHandler::IpcamHandler(std::string path, std::string imagePrefix){
    this->sourcePath = this->pathPostfix + path + "/" + imagePrefix + this->imagePostfix;
}

cv::Mat IpcamHandler::getCurrentImage(){
    return this->currentImage;

}
cv::Mat IpcamHandler::getPreviousImage(){
    return this->previousImage;

}

void IpcamHandler::setPath(std::string path, std::string imagePrefix){
    this->sourcePath = this->pathPostfix + path + "/" + imagePrefix + this->imagePostfix;
    
}

void IpcamHandler::loadNextImage(){
    this->targetPath = this->workingImagePath + std::to_string(this->numberOfRecievedImages) + this->imagePostfix;

    std::string popenInstruction = "wget -q " + this->sourcePath + " -O " + this->targetPath;
    const char *command = popenInstruction.c_str();
    FILE *file = popen(command, "r");
    fclose(file);
    
    if(!this->currentImage.empty()){
        this->previousImage = this->currentImage;
    }
    
    this->currentImage = cv::imread(this->targetPath);

    cv::namedWindow( "Image output" );
    cv::imshow("Image output", this->currentImage); cv::waitKey(5); // here's your car ;)

    this->numberOfRecievedImages++;
}


IpcamHandler::~IpcamHandler(){};

