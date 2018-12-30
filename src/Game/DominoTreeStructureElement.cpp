//
// Created by osboxes on 11/29/18.
//

#include "DominoTreeStructureElement.h"

void DominoTreeStructureElement::mount(DominoTreeStructureElement dominoTreeStructureElement) {
    if (this->hasFreeMountPoints())
        mountedElements.emplace_front(dominoTreeStructureElement);
    else throw std::exception();
}

bool DominoTreeStructureElement::hasMountedElements() {
    return !mountedElements.empty();
}

bool DominoTreeStructureElement::hasFreeMountPoints() {
    return ((element.isNormalStone()   && mountedElements.size()<NORMALMOUNTPOINTAMOUNT) ||
            (element.isDoubletsStone() && mountedElements.size()<DOUBLETMOUNTPOINTAMOUNT));
}

DominoPiece DominoTreeStructureElement::getElement()  {
    return element;
}

void DominoTreeStructureElement::setElement( DominoPiece &element) {
    DominoTreeStructureElement::element = element;
}

 std::list<DominoTreeStructureElement>* DominoTreeStructureElement::getMountedElements()  {
    return &mountedElements;
}

void DominoTreeStructureElement::setMountedElements( std::list<DominoTreeStructureElement> &mountedElements) {
    DominoTreeStructureElement::mountedElements = mountedElements;
}

std::list<int> DominoTreeStructureElement::getApplicablePips() {
    std::list<int> pips;

    int mountedStonesToA=0;
    int mountedStonesToB=0;
    std::list <DominoTreeStructureElement> :: iterator it;
    for(it = mountedElements.begin(); it != mountedElements.end(); ++it){
        if(cv::norm(cv::Mat(element.getHalfA().getRect().center),cv::Mat(it.operator*().getElement().getCenter())) < cv::norm(cv::Mat(getElement().getHalfB().getRect().center),cv::Mat(it.operator*().getElement().getCenter())))
            mountedStonesToA++;
        else
            mountedStonesToB++;
    }
    if (mountedStonesToA==0)
        pips.push_front(element.getHalfA().getNumber());
    if (mountedStonesToB==0)
        pips.push_front(element.getHalfB().getNumber());
    return pips;
}
