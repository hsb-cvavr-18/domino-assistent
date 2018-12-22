//
// Created by osboxes on 12/3/18.
//
#include <iostream>
#include "DominoCV.h"

// color for drawing into img
cv::Scalar brightColor = cv::Scalar(255, 0, 242);

dominoPiece detectPiece(cv::Mat previousImg, cv::Mat currentImg) {

    AbstractImgDebugPrinter *printer = IImgDebugPrinterFactory().getPrinter();

    // will hold the frame of the origin image for output with additional information
    cv::Mat unprocessedFrame = currentImg.clone();
    // will hold our frame to do image processing on
    cv::Mat frame = currentImg;
    // will hold the difference of the current and the previous frame
    cv::Mat diffframe;


    /***************************************************************************
     * Seperating new domino block and reduce date for further processing
     */
    // convert to grayscale
    cvtColor(frame, frame, CV_BGR2GRAY);
    cvtColor(previousImg, previousImg, CV_BGR2GRAY);

    // remove background (previous image -image)
    absdiff(frame, previousImg,frame);
    diffframe = frame.clone();
    imwrite("domino_diff.jpg", frame);

    // threshold - converting to binary image
    int thresh = 42;
    threshold(frame, frame,thresh, 255, cv::THRESH_BINARY );
    imwrite("domino_bin.jpg", frame);

    // applying blur filter - makes edges smoother an closes gaps for continuous edges
    blur(frame.clone(), frame, cv::Size(3, 3));
    imwrite("domino_bin_blur.jpg", frame);

    // applying canny edge filter
    Canny(frame, frame, 2, 2 * 2, 3, false);
    imwrite("domino_canny.jpg", frame);

    imwrite("frame.jpg", frame);


    /***************************************************************************
     * Find new Domino block in image
     */
    //find all first level contours (CV_RETR_EXTERNAL)
    vector<vector<cv::Point> > pieceContours;
    vector<cv::Vec4i> diceHierarchy;
    findContours(frame.clone(), pieceContours, diceHierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if(pieceContours.size() <= 0) {
        throw std::runtime_error("No contours found");
    }

    //Find the largest contour and create a rotated rect around it
    cv::RotatedRect minAreaRotatedRect = getRotatedRectOflargestContour(pieceContours, frame.size());

    //need the angle to be corrected ?
    const float EPS = 0.001;
    bool isCorrectionOfAngleNeeded = ((getCorrectedAngle(minAreaRotatedRect) -  minAreaRotatedRect.angle) > EPS) ? (true): (false);
    cout << "isCorrectionOfAngleNeeded: " << isCorrectionOfAngleNeeded << endl;

    //get bounding box of rotated rect
    cv::Rect dominoBoundsRect = minAreaRotatedRect.boundingRect();

    //Just for the show (debugging)
    cv::Mat rotated = unprocessedFrame.clone();
    drawRotatedRect(rotated, minAreaRotatedRect);
    imwrite("domino_rotated_rect.jpg", rotated);

    //Cut out wanted domino - containeing only wanted Domino
    cv::Mat diceROI = diffframe(dominoBoundsRect);
    imwrite("rect.jpg", diceROI);


    /***************************************************************************
     * Get Pips of each half of the domino block
     */
    /***************************************************************************
     * Find and define halfs
     */
    dominoHalf half1;
    dominoHalf half2;

    //get corners of whole block
    cv::Point2f cornerPoints[4];
    minAreaRotatedRect.points(cornerPoints);

    //set random corner as startCorner
    cv::Point2f startCornerHalf1 = cornerPoints[0];
    cv::Point2f startCornerHalf2 = cornerPoints[2];

    thread t1(getDominoHalf, &half1, diffframe.clone(), cornerPoints, startCornerHalf1, isCorrectionOfAngleNeeded);
    thread t2(getDominoHalf, &half2, diffframe.clone(), cornerPoints, startCornerHalf2, isCorrectionOfAngleNeeded);
    t1.join();
    t2.join();

    cv::Point2f half1CornerPoints[4];
    cv::Point2f half2CornerPoints[4];
    half1.rect.points(half1CornerPoints);
    half2.rect.points(half2CornerPoints);
    string numberCharactermapping[4] = {"A","B","C","D"};
    for(int i = 0; i < 4 ; i++){
        circle(unprocessedFrame, half1CornerPoints[i], 3, brightColor, 1, 8, 0);
        circle(unprocessedFrame, half2CornerPoints[i], 3, brightColor, 1, 8, 0);
        putText(unprocessedFrame, "1"+numberCharactermapping[i],  half1CornerPoints[i],  0, 1, brightColor,1, 8,false );
        putText(unprocessedFrame, "2"+numberCharactermapping[i],  half2CornerPoints[i],  0, 1, brightColor, 1, 8, false );
    }


    unprocessedFrame = drawPipCount(half1, unprocessedFrame);
    unprocessedFrame = drawPipCount(half2, unprocessedFrame);

    unprocessedFrame = colorizeHalf(half1, unprocessedFrame);
    unprocessedFrame = colorizeHalf(half2, unprocessedFrame);

    printer->printImage("Result", unprocessedFrame);
    imwrite("domino_result.jpg", unprocessedFrame);

    dominoPiece piece;
    piece.a = half2;
    piece.b = half1;
    return piece;
}

std::vector<dominoPiece> detectPlayerDominoPieces(cv::Mat firstImg, cv::Mat currentImg) {
    vector<dominoPiece> pieces;
    ImageClipping *imageClipper = new ImageClipping(PlayerPosition::POS_LEFT, 15,12.5);
    imageClipper->setSourceImage(currentImg);
    cv::Mat playerImg = imageClipper->getPlayersAreaImage();
    cv::Mat playingFieldMarked = imageClipper->getOverlayedImage();

    std::vector<std::future<dominoPiece>> futures;
    auto *threads = new std::thread[NUMBER_OF_PLAYER_BLOCKS];
    for(int i = 0; i < NUMBER_OF_PLAYER_BLOCKS; i++) {
        //threads[i] = std::thread(getPlayerDominoPiece, imageClipper, firstImg, currentImg, i);
        futures.push_back(std::async(std::launch::async, &getPlayerDominoPiece, imageClipper, firstImg, currentImg, i));
    }

    for (auto& f : futures)
    {
        try
        {
            dominoPiece res = f.get();
            pieces.push_back(res);
            std::cout << &res << std::endl;
        }
        catch(exception & e)
        {
            cout<<e.what()<<endl;
        }
    }

    /*for(int i = 0; i < NUMBER_OF_PLAYER_BLOCKS; i++) {
        std::future<dominoPiece> ret = std::async(&getPlayerDominoPiece, imageClipper, firstImg, currentImg, i);
        try {
            pieces.push_back(ret.get());
        } catch (std::runtime_error const& e){
            std::cerr << e.what() << std::endl;
        } catch( const string& e ) {
            std::cerr << e.c_str() << std::endl;
        } catch( const std::exception& e ) {
            std::cerr << e.what() << std::endl;
        } catch( ... ) {
            // ensure destructors of auto objects are called
        }

    }*/

    return pieces;
}

dominoPiece getPlayerDominoPiece(ImageClipping *imageClipper, cv::Mat firstImg, cv::Mat currentImg, int blockNumber) {

    cv::Rect fieldRect = imageClipper->getPlayerDominiBlock(blockNumber);
    cv::imwrite("debug_firstImg.jpg", firstImg);
    cv::imwrite("debug_currentImg.jpg", currentImg);
    cv::Mat previousField = cutPlayerBlock(firstImg, fieldRect);
    cv::Mat currentField = cutPlayerBlock(currentImg, fieldRect);

    std::ostringstream name;
    name << "player_domino_field_" << blockNumber << ".jpg" ;
    std::cout << name.str();
    cv::imwrite(name.str(), currentField);
    return detectPiece(previousField, currentField);

}

cv::Mat cutPlayerBlock(cv::Mat image, cv::Rect rect) {
    cv::Mat playerBlock = image(rect);
    return playerBlock;
}

