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
        auto dominoTreeStructureElement = DominoTreeStructureElement(stone);
        rootElement.mount(DominoTreeStructureElement(stone));
    };

    std::list<DominoPiece> getAvailableMountPoints() {
        return getAvailableMountPoints(rootElement);
    }
    
    std::list<DominoPiece> getAvailableMountPointsForPassedStone(DominoPiece stone) {
        const std::list<DominoPiece> availableMountPoints = getAvailableMountPoints();
        std::list<DominoPiece> filteredMountPoints;

        std::copy_if (availableMountPoints.begin(), availableMountPoints.end(), std::back_inserter(filteredMountPoints),
                [stone](DominoPiece dominoPiece) {
                    return dominoPiece.isApplicableTo(stone);
                }
        );


        return filteredMountPoints;
    }

private:
    std::list<DominoPiece> getAvailableMountPoints(DominoTreeStructureElement element){
        std::list<DominoPiece> availableMountPoints;

        if (element.hasFreeMountPoints()){
            availableMountPoints.emplace_front(element.getElement());
        }

        if (element.hasMountedElements()){
            std::list<DominoTreeStructureElement> mountedElements= element.getMountedElements();

            std::list <DominoTreeStructureElement> :: iterator it;
            for(it = mountedElements.begin(); it != mountedElements.end(); ++it){

                std::list<DominoPiece> additionalMountPoints = getAvailableMountPoints(*it);

                std::list <DominoPiece> :: iterator it;
                for(it = additionalMountPoints.begin(); it != additionalMountPoints.end(); ++it){
                    availableMountPoints.emplace_front(*it);
                }
            }
        }

        return availableMountPoints;
    };
};


#endif //PROJECT_PLAYGROUND_H
