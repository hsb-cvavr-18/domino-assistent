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

    DominoTreeStructureElement getNearestElement(DominoTreeStructureElement element,std::list<DominoTreeStructureElement> mountPoints );

    RecommendedMove recommendMove(std::list<DominoPiece> userStones);

    std::list<DominoPiece> getAvailableMountPoints();
    
    std::list<DominoPiece> getAvailableMountPointsForPassedStone(DominoPiece stone);

private:
    std::list<DominoPiece> getAvailableMountPoints(DominoTreeStructureElement element);
    std::list<DominoTreeStructureElement> getAvailableMountPointsForPassedElement(DominoTreeStructureElement stone);
    std::list<DominoTreeStructureElement> getAvailableMountPointsAsElements(DominoTreeStructureElement element);
    RecommendedMove stupidMove(std::list<DominoPiece> userStones);
    RecommendedMove fistMove(std::list<DominoPiece> userStones);
    };


#endif //PROJECT_PLAYGROUND_H
