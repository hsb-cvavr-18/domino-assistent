//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_PLAYGROUND_H
#define PROJECT_PLAYGROUND_H

#include "DominoTreeStructureElement.h"


struct RecommendedMove {
    DominoPiece userStone;
    DominoPiece recommendedStone;
};

class PlayGround {

private:
    DominoTreeStructureElement rootElement;

public:
    PlayGround(DominoTreeStructureElement rootElement) : rootElement(rootElement) {}
    void mountStone(DominoPiece stone);
    RecommendedMove recommendMove(std::list<DominoPiece> userStones);

private:
    DominoTreeStructureElement* getNearestElement(std::list<DominoTreeStructureElement*> mountPoints, DominoTreeStructureElement* element );
    std::list<DominoTreeStructureElement*> getAvailableMountPointsAsElements(DominoTreeStructureElement* existingElement, DominoTreeStructureElement* newStone);
    RecommendedMove stupidMove(std::list<DominoPiece> userStones);
    RecommendedMove firstMove(std::list<DominoPiece> userStones);
};
#endif //PROJECT_PLAYGROUND_H
