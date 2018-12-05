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

    void mountStone(DominoPiece stone){
        rootElement.mount(DominoTreeStructureElement(stone));
    };

    void mountStone(DominoTreeStructureElement at, DominoPiece stone) {
        at.mount(DominoTreeStructureElement(stone));
    }

    std::list<DominoTreeStructureElement> getAvailableMountPoints() {
        return getAvailableMountPoints(rootElement);
    }
    
    std::list<DominoTreeStructureElement> getAvailableMountPointsForPassedStone(DominoPiece stone) {
        const std::list<DominoTreeStructureElement> availableMountPoints = getAvailableMountPoints();
        std::list<DominoTreeStructureElement> filteredMountPoints;

        std::copy_if (availableMountPoints.begin(), availableMountPoints.end(), std::back_inserter(filteredMountPoints),
                [stone](DominoTreeStructureElement dominoPiece) {
                    return dominoPiece.getElement().isApplicableTo(stone);
                }
        );


        return filteredMountPoints;
    }

private:
    std::list<DominoTreeStructureElement> getAvailableMountPoints(DominoTreeStructureElement element){
        std::list<DominoTreeStructureElement> availableMountPoints;

        if (element.hasFreeMountPoints()){
            availableMountPoints.emplace_front(element.getElement());
        }

        if (element.hasMountedElements()){
            std::list<DominoTreeStructureElement> mountedElements= element.getMountedElements();

            std::list <DominoTreeStructureElement> :: iterator it;
            for(it = mountedElements.begin(); it != mountedElements.end(); ++it){

                std::list<DominoTreeStructureElement> additionalMountPoints = getAvailableMountPoints(*it);

                std::list <DominoTreeStructureElement> :: iterator it;
                for(it = additionalMountPoints.begin(); it != additionalMountPoints.end(); ++it){
                    availableMountPoints.emplace_front(*it);
                }
            }
        }

        return availableMountPoints;
    };
};


#endif //PROJECT_PLAYGROUND_H
