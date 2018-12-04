//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_PLAYGROUND_H
#define PROJECT_PLAYGROUND_H


#include "DominoTreeStructureElement.h"

class PlayGround {
private:
    DominoTreeStructureElement rootElement;
public:
    PlayGround(DominoTreeStructureElement rootElement) : rootElement(rootElement) {}
    void mountStone(Stone stone){
        if (rootElement.mountingWouldBeValid(&stone)){
            DominoTreeStructureElement(stone);
            rootElement.mount(stone);
        }
    };
    void getAvalailableMountPoints(){};
};


#endif //PROJECT_PLAYGROUND_H
