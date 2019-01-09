#include "main.h"

static const char *const address = "192.168.43.109:8080";
auto imageHandler = ImageHandlerFactory::getImageHandler("../../srcImgApplyPlayerStones", "apply_", Source::FILESYSTEM);

void task_main();
void task_gui();
void task_preview(std::string address);
int main(int argc, char **argv) {
    cout << "Running main" << std::endl;

    thread main_thread(task_main);
    thread gui_thread(task_gui);
    task_preview(address);
    main_thread.join();


    return EXIT_SUCCESS;
}


void task_main() {
    /***************************************************************************
   * load the Picture with new Domino and the predecessor picture
   */
    //new Domino

    auto imageHandler = ImageHandlerFactory::getImageHandler(address, "photo", Source::IP_CAM);
    cv::Mat currentImg = cv::Mat();
    cv::Mat previousImg = cv::Mat();
    PlayGround *playGround= nullptr;
    while(true) {
        //TODO: Verarbeitung der Bilder (Logik - wann wird ausgelöst, behandlung der ersten zwei Bilder etc.
        do {
            imageHandler->loadNextImage();
            currentImg = imageHandler->getCurrentImage();
            if (NULL == &currentImg || currentImg.empty()) {
                std::cout << "No more Image loaded" << std::endl;

                return;// EXIT_SUCCESS;
            }

            previousImg = imageHandler->getPreviousImage();

        } while (previousImg.empty());

        ImageClipping *imageClipper = ImageClippingFactory::getImageClipping();
        imageClipper->setSourceImage(previousImg);
        cv::Mat previousImgCropped = imageClipper->getPlayingFieldImage().roi;
        imageClipper->setSourceImage(currentImg);
        cv::Mat currentImgCropped = imageClipper->getPlayingFieldImage().roi;

        DominoPiece dominoPiece = detectPiece(previousImgCropped, currentImgCropped, imageClipper->getPlayingFieldImage().offset);
        if (playGround == nullptr)
            playGround = new PlayGround(dominoPiece);
        else
            playGround->mountStone(dominoPiece);
        cv::Mat result;
        result = cv::imread("domino_result.jpg");

        cout << "found piece: " << dominoPiece << endl;

        imageClipper->setSourceImage(currentImg);
        cv::Mat playerImg_cropped = imageClipper->getPlayersAreaImage().roi;

        const vector<DominoPiece> &dominoPlayerPieces = detectPlayerDominoPieces(imageHandler->getFirstImage(), playerImg_cropped, imageClipper);
        for(auto playerPiece : dominoPlayerPieces) {
            std::cout << "found player piece " << playerPiece << std::endl;
        }
        list<DominoPiece> dominoPlayerPiecesList(dominoPlayerPieces.begin(), dominoPlayerPieces.end());
        playGround->setUserStones(dominoPlayerPiecesList);

        cout << "User, please put " << endl;
        RecommendedMove rm = playGround->recommendMove().front();
        std::cout << rm.userStone << " onto " << rm.recommendedStone << endl;

        cv::Mat result_tmp;
        cv::hconcat(playerImg_cropped, result, result_tmp);

        cv::imwrite("result_tmp.jpg", result_tmp);
        cv::Mat result_final = result_tmp.clone();
        result_final = drawSuggestedMove(rm.userStone, rm.recommendedStone, result_tmp);
        cv::imwrite("result_final.jpg", result_final);

        gameFrames.push(result_final);
        waitForUserInput();
    }

}

void task_preview(std::string address)
{
    cv::VideoCapture vcap;
    cv::Mat image;
    float aspectRatio;
    int width = 500;
    const std::string videoStreamAddress = "http://" + address + ":8080/video";


    //open the video stream and make sure it's opened
    if(!vcap.open(videoStreamAddress)) {
        std::cout << "Error opening video stream or file" << std::endl;
        return;
    }

    ImageClipping *imageClipper = ImageClippingFactory::getImageClipping();
    auto before = std::chrono::high_resolution_clock::now();
    for(;;) {
        if(!vcap.read(image)) {
            std::cout << "No frame" << std::endl;
        } else {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = now-before;
            if(elapsed.count() > 550) {

                imageClipper->setSourceImage(image);
                cv::Mat playingFieldMarked = imageClipper->getOverlayedImage();
                previewFrames.push(playingFieldMarked);

                before = std::chrono::high_resolution_clock::now();
            }
        }
    }
}

void task_gui() {
    const int width = 500;

    namedWindow( "PlayingAreas", cv::WINDOW_NORMAL );// Create a window for display.
    cv::resizeWindow("PlayingAreas", width / 2.5, width / 2.5);


    namedWindow( "Preview", cv::WINDOW_NORMAL );// Create a window for display

    cv::resizeWindow("Preview", width, width);
    cv::moveWindow("Preview", 1920 - (width +50),20);

    for(;;) {
        cv::Mat m;
        if(previewFrames.next(m) ) {
            cv::imshow("Preview", m);
        }
        if(gameFrames.next(m) ) {
            cv::imshow("PlayingAreas", m);
        }
        cv::waitKey(25);
    }
}