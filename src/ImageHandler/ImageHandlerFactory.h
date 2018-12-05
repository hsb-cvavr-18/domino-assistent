//
// Created by osboxes on 04.12.18.
//

#ifndef PROJECT_IMAGEHANDLERFACTORY_H
#define PROJECT_IMAGEHANDLERFACTORY_H

#include <stdio.h>

#include "I_ImageHandler.h"
#include "FileImageHandler.h"
#include "IpcamHandler.h"


const int IH_PICAM = 0;
const int IH_FILE = 1;
enum class Source {IP_CAM, FILESYSTEM};

class ImageHandlerFactory
{
    public:    
    static I_ImageHandler* getImageHandler(std::string path, std::string imagePrefix, Source source = Source::IP_CAM);
};

#endif //PROJECT_IMAGEHANDLERFACTORY_H
