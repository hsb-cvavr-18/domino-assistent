//
// Created by osboxes on 04.12.18.
//

#include "ImageHandlerFactory.h"

/**
* gicves a ImageHandler for getting images from disk or from IP Cam.
* @param source the source for pictures
* @param path path to image folder or address of image
* @return images - previous and current immage
*/
I_ImageHandler* ImageHandlerFactory::getImageHandler(std::string path, std::string imagePrefix, Source source) {
    I_ImageHandler *ImageHandler;
    if(source == Source::IP_CAM){
        ImageHandler = new IpcamHandler(path,imagePrefix);
        
    }else if(source == Source::FILESYSTEM){
        ImageHandler = new FileImageHandler(path,imagePrefix);
        
    } else {ImageHandler = NULL;}
    
    return ImageHandler;
}