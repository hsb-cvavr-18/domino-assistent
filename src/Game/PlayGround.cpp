#include <utility>

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

        std::list <DominoTreeStructureElement> :: iterator itMounted;
        for(itMounted = mountedElements->begin(); itMounted != mountedElements->end(); ++itMounted){

            std::list<DominoTreeStructureElement*> additionalMountPoints = getAvailableMountPointsAsElements(&*itMounted,newStone);

            std::list <DominoTreeStructureElement*> :: iterator itAdditional;
            for(itAdditional = additionalMountPoints.begin(); itAdditional != additionalMountPoints.end(); ++itAdditional){
                availableMountPoints.emplace_front(*itAdditional);
            }
        }
    }

    return availableMountPoints;
}

// Zugvorhersage

std::list<RecommendedMove> PlayGround::recommendMove() {
    return firstMove(std::move(userStones)); // empty if no possible move.
}

std::list<RecommendedMove> PlayGround::firstMove(std::list<DominoPiece> userStones) {
    std::list<RecommendedMove> recommendedMoves;
    RecommendedMove move;

    std::list <DominoPiece> :: iterator it;
    for(it = userStones.begin(); it != userStones.end(); ++it){
        move.userStone=*it;
        DominoTreeStructureElement usrPieceElement = DominoTreeStructureElement(*it);
        std::list<DominoTreeStructureElement*> availableMountPoints = getAvailableMountPointsAsElements(&rootElement,&usrPieceElement);

        if(!availableMountPoints.empty()){
            move.recommendedStone = availableMountPoints.front()->getElement();
            recommendedMoves.push_back(move);
        }
    }
    return recommendedMoves;
}

std::list<DominoPiece> PlayGround::getUserStones() {return userStones;}

void PlayGround::setUserStones(std::list<DominoPiece> userStones) { PlayGround::userStones = std::move(userStones);}

void PlayGround::addUserStone(DominoPiece newStone) {userStones.push_back(newStone);}

void PlayGround::removeUserStone(DominoPiece newStone) {userStones.remove(newStone);}
