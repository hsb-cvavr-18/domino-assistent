//
// Created by osboxes on 05.12.18.
//

#include "FileImageHandler.h"


FileImageHandler::FileImageHandler(std::string path, std::string imagePrefix){
    this->sourcePath = this->pathPostfix + path + "/" + imagePrefix;
    std::cout<< this->sourcePath <<std::endl;
}

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
    if(this->testFileExistence(imagePath)) {
        this->previousImage = this->currentImage;
        this->currentImage = cv::imread(imagePath);
    } else { this->currentImage = NULL; return;}



    cv::namedWindow( "Image output" );
    cv::imshow("Image output", this->currentImage); cv::waitKey(5); // here's your car ;)

    this->numberOfRecievedImages++;
}

bool FileImageHandler::testFileExistence(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}



FileImageHandler::~FileImageHandler(){};