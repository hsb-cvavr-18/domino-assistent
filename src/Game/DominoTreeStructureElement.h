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
#include "Stone.h"

class DominoTreeStructureElement {
private:
    Stone element;
    std::list<DominoTreeStructureElement> mountedElements = std::list<DominoTreeStructureElement>();

public:
    DominoTreeStructureElement(Stone element) : element(element) {}

    void mount(DominoTreeStructureElement dominoTreeStructureElement) {
        if (this->hasFreeMountPoints())
            mountedElements.emplace_front(dominoTreeStructureElement);
        else throw std::exception();
    }


    void showlist(std::list<DominoTreeStructureElement*> mountedElements)
    {
        std::list <DominoTreeStructureElement*> :: iterator it;
        for(it = mountedElements.begin(); it != mountedElements.end(); ++it){

            std::cout << '\t' << *it;
        }
        std::cout << '\n';
    }

    bool hasFreeMountPoints(){
        return ((element.isNormalStone()   && mountedElements.size()<NORMALMOUNTPOINTAMOUNT) ||
                (element.isDoubletsStone() && mountedElements.size()<DOUBLETMOUNTPOINTAMOUNT));
    }
    
    const Stone getElement() const {
        return element;
    }
};


#endif //PROJECT_DOMINOTREESTRUCTUREELEMENT_H
