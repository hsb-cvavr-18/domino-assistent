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
#include "Stone.h"

class DominoTreeStructureElement {
private:
    Stone element;
    std::list<DominoTreeStructureElement*> mountedElements = std::list<DominoTreeStructureElement*>();

public:
    DominoTreeStructureElement(const Stone &stone, Stone element) : element(std::move(element)) {}

    void mountStone(Stone stone) {
        if (this->hasFreeMountPoints())
            mountedElements.emplace_front(stone);
    }

    bool mountingWouldBeValid(){
        // TODO: Lageprüfunq ([2,4][4,5], dann kann nicht an erster vier angelegt werden)

    }

    bool hasFreeMountPoints(){
        return ((element.isNormalStone()   && mountedElements.size()<NORMALMOUNTPOINTAMOUNT) ||
            (element.isDoubletsStone() && mountedElements.size()<DOUBLETMOUNTPOINTAMOUNT));
    }

};


#endif //PROJECT_DOMINOTREESTRUCTUREELEMENT_H
