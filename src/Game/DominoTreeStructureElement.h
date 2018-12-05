#include <utility>

//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_DOMINOTREESTRUCTUREELEMENT_H
#define PROJECT_DOMINOTREESTRUCTUREELEMENT_H

// 2 Anlegestellen für normale Steine, 4 für PaschStein
#define NORMALMOUNTPOINTAMOUNT  2
#define DOUBLETMOUNTPOINTAMOUNT 4

#include <iterator>
#include <iostream>
#include "DominoPiece.h"

class DominoTreeStructureElement {
private:
    DominoPiece element;
    std::list<DominoTreeStructureElement> mountedElements = std::list<DominoTreeStructureElement>();

public:
    DominoTreeStructureElement(DominoPiece element) : element(element) {
    }

    void mount(DominoTreeStructureElement dominoTreeStructureElement) {
        if (this->hasFreeMountPoints())
            mountedElements.emplace_front(dominoTreeStructureElement);
        else throw std::exception();
    }


    bool hasMountedElements(){
        return !mountedElements.empty();
    }

    bool hasFreeMountPoints(){
        return ((element.isNormalStone()   && mountedElements.size()<NORMALMOUNTPOINTAMOUNT) ||
                (element.isDoubletsStone() && mountedElements.size()<DOUBLETMOUNTPOINTAMOUNT));
    }

    const DominoPiece getElement() const {
        return element;
    }

    void setElement(const DominoPiece &element) {
        DominoTreeStructureElement::element = element;
    }

    const std::list<DominoTreeStructureElement> &getMountedElements() const {
        return mountedElements;
    }

    void setMountedElements(const std::list<DominoTreeStructureElement> &mountedElements) {
        DominoTreeStructureElement::mountedElements = mountedElements;
    }
};


#endif //PROJECT_DOMINOTREESTRUCTUREELEMENT_H
