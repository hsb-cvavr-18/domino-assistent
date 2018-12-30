//
// Created by osboxes on 12/30/18.
//

#include "ImageClippingFactory.h"

ImageClipping* ImageClippingFactory::getImageClipping() {
    return new ImageClipping(PlayerPosition::POS_LEFT, 15,12.5);
}