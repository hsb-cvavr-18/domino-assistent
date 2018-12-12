#include "main.h"

void task_main();
void task_gui();
void task_preview(std::string address);
int main(int argc, char **argv) {
    cout << "Running main" << std::endl;

    thread main_thread(task_main);
    thread gui_thread(task_gui);
    task_preview("192.168.43.152");
    main_thread.join();


    return EXIT_SUCCESS;
}


void task_main() {
    /***************************************************************************
   * load the Picture with new Domino and the predecessor picture
   */
    //new Domino
    auto imageHandler = ImageHandlerFactory::getImageHandler("../../srcImg", "gestell_", Source::FILESYSTEM);
    //auto imageHandler = ImageHandlerFactory::getImageHandler("192.168.178.79:8080", "photo", Source::IP_CAM);
    cv::Mat currentImg = cv::Mat();
    cv::Mat previousImg = cv::Mat();
    ImageClipping *imageClipper = new ImageClipping(PlayerPosition::POS_LEFT, 15,12.5);

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


        imageClipper->setSourceImage(currentImg);
        cv::Mat playerImg = imageClipper->getPlayersAreaImage();
        cv::Mat playingFieldMarked = imageClipper->getOverlayedImage();

        gameFrames.push(playingFieldMarked);

        const dominoPiece &dominoPiece = detectPiece(previousImg, currentImg);

        cout << "pipcount half 1: " << dominoPiece.a.pips << endl;
        cout << "pipcount half 2: " << dominoPiece.b.pips << endl;

        std::cout << "Enter key to take next img" << std::endl;
        getchar();
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

    for(;;) {
        if(!vcap.read(image)) {
            std::cout << "No frame" << std::endl;
        } else {
            previewFrames.push(image);
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