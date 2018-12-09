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

    void mountStone(DominoPiece stone);;

    std::list<DominoPiece> getAvailableMountPoints();
    
    std::list<DominoPiece> getAvailableMountPointsForPassedStone(DominoPiece stone);

private:
    std::list<DominoPiece> getAvailableMountPoints(DominoTreeStructureElement element);;
};


#endif //PROJECT_PLAYGROUND_H
