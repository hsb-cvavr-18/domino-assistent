//
// Created by osboxes on 04.12.18.
//

#include "IpcamHandler.h"

IpcamHandler::IpcamHandler(std::string path, std::string imagePrefix){
    this->sourcePath = this->pathPostfix + path + "/" + imagePrefix + this->imagePostfix;
}

cv::Mat IpcamHandler::getFirstImage() {
    return this->firstImage;
};

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
    try{
        std::cout << "Trying to load Image from IP Cam" <<std::endl;
        FILE *file = popen(command, "r");
        fclose(file);

    } catch(const std::exception& e){
        std::cout << "ERROR while loading image from IP Cam: " << e.what()  << std::endl;
        return;
    }


    if(!this->currentImage.empty()){
        this->previousImage = this->currentImage;
    }

    this->currentImage = cv::imread(this->targetPath);
    this->setFirstImage(this->currentImage);
    if(!this->currentImage.empty()){
        this->numberOfRecievedImages++;
    }else{
        std::cout << "Could not Load Next Image!" << std::endl;
    }



}


IpcamHandler::~IpcamHandler(){}


