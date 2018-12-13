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

std::list<DominoPiece> PlayGround::getAvailableMountPoints() {
    return getAvailableMountPoints(rootElement);
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

std::list<DominoPiece> PlayGround::getAvailableMountPointsForPassedStone(DominoPiece stone) {
    const std::list<DominoPiece> availableMountPoints = getAvailableMountPoints();
    std::list<DominoPiece> filteredMountPoints;

    std::copy_if (availableMountPoints.begin(), availableMountPoints.end(), std::back_inserter(filteredMountPoints),
                  [stone](DominoPiece dominoPiece) {
                      return dominoPiece.isApplicableTo(stone);
                  }
    );


    return filteredMountPoints;
}

std::list<DominoPiece> PlayGround::getAvailableMountPoints(DominoTreeStructureElement element) {
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
}

RecommendedMove PlayGround::recommendMove(std::list<DominoPiece> userStones) {
    return stupidMove(userStones);
    }
}

RecommendedMove PlayGround::stupidMove(std::list<DominoPiece> userStones) {
    RecommendedMove move;
    move.userStone = userStones.front();
    move.recommendedStone = rootElement.getElement();
    return RecommendedMove();
}




