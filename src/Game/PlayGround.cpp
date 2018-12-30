#include <utility>

#include <utility>

//
// Created by osboxes on 11/29/18.
//

#include "PlayGround.h"

void PlayGround::mountStone(DominoPiece stone) {
    DominoTreeStructureElement element = DominoTreeStructureElement(std::move(stone));
    std::list<DominoTreeStructureElement> availableMountPoints = getAvailableMountPointsForPassedElement(element);
    if(!availableMountPoints.empty()){
        DominoTreeStructureElement nearestElement =  getNearestElement(element,availableMountPoints);
        nearestElement.mount(element);
    }
}

DominoTreeStructureElement PlayGround::getNearestElement(DominoTreeStructureElement element,std::list<DominoTreeStructureElement> mountPoints ){
    cv::Point2f centerOfElement = element.getElement().getCenter();

    std::list <DominoTreeStructureElement> :: iterator it;
    DominoTreeStructureElement nearestElement = *mountPoints.begin();
    for(it = mountPoints.begin(); it != mountPoints.end(); ++it){
        if(cv::norm(cv::Mat(centerOfElement),cv::Mat(it.operator*().getElement().getCenter())) < cv::norm(cv::Mat(centerOfElement),cv::Mat(nearestElement.getElement().getCenter())))
            nearestElement = *it;
    }
    return nearestElement;
}

std::list<DominoTreeStructureElement> PlayGround::getAvailableMountPointsForPassedElement(DominoTreeStructureElement stone){

    std::list<DominoTreeStructureElement> availableMountPoints = getAvailableMountPointsAsElements(stone);
    std::list<DominoTreeStructureElement> filteredMountPoints;

    std::copy_if (availableMountPoints.begin(), availableMountPoints.end(), std::back_inserter(filteredMountPoints),
                  [stone](DominoTreeStructureElement dominoTreeStructureElement) {
                      return dominoTreeStructureElement.isApplicableTo(stone);
                  }
    );

    return filteredMountPoints;
}

std::list<DominoTreeStructureElement> PlayGround::getAvailableMountPointsAsElements(DominoTreeStructureElement element) {

    std::list<DominoTreeStructureElement> availableMountPoints;

    if (element.hasFreeMountPoints()){
        availableMountPoints.emplace_front(element);
    }

    if (element.hasMountedElements()){
        std::list<DominoTreeStructureElement> mountedElements= element.getMountedElements();

        std::list <DominoTreeStructureElement> :: iterator it;
        for(it = mountedElements.begin(); it != mountedElements.end(); ++it){

            std::list<DominoTreeStructureElement> additionalMountPoints = getAvailableMountPointsAsElements(*it);

            std::list <DominoTreeStructureElement> :: iterator it;
            for(it = additionalMountPoints.begin(); it != additionalMountPoints.end(); ++it){
                availableMountPoints.emplace_front(*it);
            }
        }
    }

    return availableMountPoints;
}

// Zugvorhersage

RecommendedMove PlayGround::recommendMove(std::list<DominoPiece> userStones) {
    return stupidMove(std::move(userStones));
}

RecommendedMove PlayGround::stupidMove(std::list<DominoPiece> userStones) {
    RecommendedMove move;
    move.userStone = userStones.front();
    move.recommendedStone = rootElement.getElement();
    return move;
}

RecommendedMove PlayGround::firstMove(std::list<DominoPiece> userStones) {
    RecommendedMove move;
    std::list<DominoTreeStructureElement> availableMountPoints = getAvailableMountPointsAsElements(rootElement);

    std::list <DominoPiece> :: iterator usrPieces;
    std::list <DominoTreeStructureElement> :: iterator elements;
    for(usrPieces = userStones.begin(); usrPieces != userStones.end(); ++usrPieces){
        for(elements = availableMountPoints.begin(); elements != availableMountPoints.end(); ++usrPieces){

        }
    }
    return move;
}

