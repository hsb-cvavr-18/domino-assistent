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

 std::list<DominoTreeStructureElement> &DominoTreeStructureElement::getMountedElements()  {
    return mountedElements;
}

void DominoTreeStructureElement::setMountedElements( std::list<DominoTreeStructureElement> &mountedElements) {
    DominoTreeStructureElement::mountedElements = mountedElements;
}
