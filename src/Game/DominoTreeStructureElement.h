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

    DominoPiece getElement() ;

    void setElement( DominoPiece &element);

    std::list<DominoTreeStructureElement>* getMountedElements() ;

    void setMountedElements( std::list<DominoTreeStructureElement> &mountedElements);

    bool isApplicableTo(DominoTreeStructureElement newElement){
        return element.isApplicableTo(newElement.getElement());
    }

    std::list<int> getApplicablePips();

};


#endif //PROJECT_DOMINOTREESTRUCTUREELEMENT_H
