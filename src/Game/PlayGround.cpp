#include <utility>

#include <utility>

//
// Created by osboxes on 11/29/18.
//

#include "PlayGround.h"

void PlayGround::mountStone(DominoPiece stone) {
    DominoTreeStructureElement element = DominoTreeStructureElement(std::move(stone));
    std::list<DominoTreeStructureElement*> availableMountPoints = getAvailableMountPointsAsElements(&rootElement,&element);
    if(!availableMountPoints.empty()){
        DominoTreeStructureElement* nearestElement =  getNearestElement(availableMountPoints,&element);
        nearestElement->mount(element);
    }
}

DominoTreeStructureElement* PlayGround::getNearestElement(std::list<DominoTreeStructureElement*> mountPoints,DominoTreeStructureElement* element ){
    cv::Point2f centerOfElement = element->getElement().getCenter();

    std::list <DominoTreeStructureElement*> :: iterator it;
    DominoTreeStructureElement* nearestElement = *mountPoints.begin();
    for(it = mountPoints.begin(); it != mountPoints.end(); ++it){
        if(cv::norm(cv::Mat(centerOfElement),cv::Mat(it.operator*()->getElement().getCenter())) < cv::norm(cv::Mat(centerOfElement),cv::Mat(nearestElement->getElement().getCenter())))
            nearestElement = *it;
    }
    return nearestElement;
}

std::list<DominoTreeStructureElement*> PlayGround::getAvailableMountPointsAsElements(DominoTreeStructureElement* existingElement, DominoTreeStructureElement* newStone) {

    std::list<DominoTreeStructureElement*> availableMountPoints;

    if (existingElement->hasFreeMountPoints() && newStone->isApplicableTo(*existingElement)){
        availableMountPoints.emplace_front(existingElement);
    }

    if (existingElement->hasMountedElements()){
        std::list<DominoTreeStructureElement>* mountedElements= existingElement->getMountedElements();

        std::list <DominoTreeStructureElement> :: iterator it;
        for(it = mountedElements->begin(); it != mountedElements->end(); ++it){

            std::list<DominoTreeStructureElement*> additionalMountPoints = getAvailableMountPointsAsElements(&*it,newStone);

            std::list <DominoTreeStructureElement*> :: iterator it;
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

    std::list <DominoPiece> :: iterator it;
    for(it = userStones.begin(); it != userStones.end(); ++it){
        DominoTreeStructureElement usrPieceElement = DominoTreeStructureElement(*it);
        std::list<DominoTreeStructureElement*> availableMountPoints = getAvailableMountPointsAsElements(&rootElement,&usrPieceElement);

        std::list <DominoTreeStructureElement*> :: iterator it;
        for(it = availableMountPoints.begin(); it != availableMountPoints.end(); ++it){

        }
    }
    return move;
}

