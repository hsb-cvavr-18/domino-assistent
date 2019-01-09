//
// Created by osboxes on 11/29/18.
//

#include "DominoTreeStructureElement.h"
#include <math.h>       /* pow */

void DominoTreeStructureElement::mount(DominoTreeStructureElement dominoTreeStructureElement) {
    if (this->hasFreeMountPoints()) {
        /* blocking mounted dominoHalfs by distance*/
        if(cv::norm(cv::Mat(element.getCenter()),cv::Mat(dominoTreeStructureElement.getElement().getHalfA().getRect().center)) <
           cv::norm(cv::Mat(element.getCenter()),cv::Mat(dominoTreeStructureElement.getElement().getHalfB().getRect().center))){
            this->element.block(dominoTreeStructureElement.element.getHalfA().getNumber());
            dominoTreeStructureElement.element.block(dominoTreeStructureElement.element.getHalfA().getNumber());
        } else{
            this->element.block(dominoTreeStructureElement.element.getHalfB().getNumber());
            dominoTreeStructureElement.element.block(dominoTreeStructureElement.element.getHalfB().getNumber());
        }
        //dominoTreeStructureElement.mountExistingStone(*this);
        mountedElements.emplace_front(dominoTreeStructureElement);
    }

    else throw std::exception();
}

bool DominoTreeStructureElement::hasMountedElements() {return !mountedElements.empty();}

bool DominoTreeStructureElement::hasFreeMountPoints() {
    return ((element.isNormalStone()   && mountedElements.size()<NORMALMOUNTPOINTAMOUNT) ||
            (element.isDoubletsStone() && mountedElements.size()<DOUBLETMOUNTPOINTAMOUNT));
}

DominoPiece DominoTreeStructureElement::getElement()  {return element;}

std::list<DominoTreeStructureElement>* DominoTreeStructureElement::getMountedElements()  {return &mountedElements;}

bool DominoTreeStructureElement::isApplicableTo(DominoTreeStructureElement newElement) {
    return this->hasFreeMountPoints() && element.isApplicableTo(newElement.getElement());
}

void DominoTreeStructureElement::mountExistingStone(DominoTreeStructureElement dominoTreeStructureElement) {
    if (this->hasFreeMountPoints()) {
        /* blocking mounted dominoHalfs by distance*/
        if(cv::norm(cv::Mat(element.getCenter()),cv::Mat(dominoTreeStructureElement.getElement().getHalfA().getRect().center)) <
           cv::norm(cv::Mat(element.getCenter()),cv::Mat(dominoTreeStructureElement.getElement().getHalfB().getRect().center))){
            this->element.block(dominoTreeStructureElement.element.getHalfA().getNumber());
            dominoTreeStructureElement.element.block(dominoTreeStructureElement.element.getHalfA().getNumber());
        } else{
            this->element.block(dominoTreeStructureElement.element.getHalfB().getNumber());
            dominoTreeStructureElement.element.block(dominoTreeStructureElement.element.getHalfB().getNumber());
        }
        mountedElements.emplace_front(dominoTreeStructureElement);
    }
    else throw std::exception();

}

