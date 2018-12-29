//
// Created by osboxes on 05.12.18.
//

#include "FileImageHandler.h"


FileImageHandler::FileImageHandler(std::string path, std::string imagePrefix){
    this->sourcePath = this->pathPostfix + path + "/" + imagePrefix;

}

cv::Mat FileImageHandler::getFirstImage() {
    return this->firstImage;
};

cv::Mat FileImageHandler::getCurrentImage(){
    return this->currentImage;

}
cv::Mat FileImageHandler::getPreviousImage(){
    return this->previousImage;

}

void FileImageHandler::setPath(std::string path, std::string imagePrefix){
    this->sourcePath = this->pathPostfix + path + "/" + imagePrefix + this->imagePostfix;

}

void FileImageHandler::loadNextImage(){

    std::string imagePath = this->sourcePath  + std::to_string(this->numberOfRecievedImages)+ this->imagePostfix;
    std::cout<< imagePath  <<std::endl;
    try {
        if (this->testFileExistence(imagePath)) {

            this->previousImage = this->currentImage;
            this->currentImage = cv::imread(imagePath);
            this->setFirstImage(this->currentImage);
        } else {
            this->currentImage = cv::Mat();
            return;
        }
    } catch(const std::exception& e){
        std::cout << "ERROR while loading image: " << e.what()  << std::endl;
    }

    if(!this->currentImage.empty()){
        this->numberOfRecievedImages++;
    }else{
        std::cout << "Could not Load Next Image!" << std::endl;
    }

    //this->numberOfRecievedImages++;
}

bool FileImageHandler::testFileExistence(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        std::cout << "ERROR image doesnt exist: " <<  name << std::endl;
        return false;
    }
}

FileImageHandler::~FileImageHandler(){}


