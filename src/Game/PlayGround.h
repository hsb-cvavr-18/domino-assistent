#include <utility>

//
// Created by osboxes on 11/29/18.
//

#ifndef PROJECT_PLAYGROUND_H
#define PROJECT_PLAYGROUND_H

#include "DominoTreeStructureElement.h"
#include "DominoPiece.h"

struct RecommendedMove {
    DominoPiece userStone;
    DominoPiece recommendedStone;
};

class PlayGround {

private:
    DominoTreeStructureElement rootElement;
    std::list<DominoPiece> userStones;
public:
    PlayGround(DominoTreeStructureElement rootElement) : rootElement(std::move(rootElement)) {}
    void mountStone(DominoPiece stone);
    std::list<RecommendedMove> recommendMove();

    std::list<DominoPiece> getUserStones();
    void setUserStones(std::list<DominoPiece> userStones);
    void addUserStone(DominoPiece newStone);
    void removeUserStone(DominoPiece newStone);

private:
    DominoTreeStructureElement* getNearestElement(std::list<DominoTreeStructureElement*> mountPoints, DominoTreeStructureElement* element );
    std::list<DominoTreeStructureElement*> getAvailableMountPointsAsElements(DominoTreeStructureElement* existingElement, DominoTreeStructureElement* newStone);

    std::list<RecommendedMove> firstMove(std::list<DominoPiece> userStones);
};
#endif //PROJECT_PLAYGROUND_H
