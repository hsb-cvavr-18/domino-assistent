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

    void mount(DominoTreeStructureElement dominoTreeStructureElement);


    bool hasMountedElements();

    bool hasFreeMountPoints();

    const DominoPiece getElement() const;

    void setElement(const DominoPiece &element);

    const std::list<DominoTreeStructureElement> &getMountedElements() const;

    void setMountedElements(const std::list<DominoTreeStructureElement> &mountedElements);
};


#endif //PROJECT_DOMINOTREESTRUCTUREELEMENT_H
