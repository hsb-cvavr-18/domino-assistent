// std lib
#include <iostream>
#include <sstream>

// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>


static const int DOMINO_PIECE_AREA_MIN = 1000;
static const int DOMINO_PIECE_AREA_MAX = 10000;

unsigned long countPips(cv::Mat dice) {

	// resize
	cv::resize(dice, dice, cv::Size(150, 150));

	// convert to grayscale
	cvtColor(dice, dice, CV_BGR2GRAY);

	// threshold
	cv::threshold(dice, dice, 150, 255, cv::THRESH_BINARY | CV_THRESH_OTSU);

	// floodfill
	cv::floodFill(dice, cv::Point(0, 0), cv::Scalar(0));
	cv::floodFill(dice, cv::Point(0, 149), cv::Scalar(0));
	cv::floodFill(dice, cv::Point(149, 0), cv::Scalar(0));
	cv::floodFill(dice, cv::Point(149, 149), cv::Scalar(0));

	// show
	cv::namedWindow("processed", true);
	cv::imshow("processed", dice); cv::waitKey(0);

	// search for blobs
	cv::SimpleBlobDetector::Params params;

	cv::Mat diceTemp;
    dice.copyTo(diceTemp);
	cv::GaussianBlur(dice, diceTemp, cv::Size(9,9), 3.3);
	dice = diceTemp;

    // show
    cv::namedWindow("blur", true);
    cv::imshow("blur", dice); cv::waitKey(0);

	// filter by interia defines how elongated a shape is.
	//params.filterByInertia = true;
	//params.minInertiaRatio = 0.5;

    params.filterByColor = true;
    params.blobColor = 255;

	// will hold our keyponts
	std::vector<cv::KeyPoint> keypoints;

	// create new blob detector with our parameters
	cv::Ptr<cv::SimpleBlobDetector> blobDetector = cv::SimpleBlobDetector::create(params);

	// detect blobs
	blobDetector->detect(dice, keypoints);

	// return number of pips
	return keypoints.size();
}

int main(int argc, char** argv) {

	// open window frame
	cv::namedWindow("frame", true);

    cv::Mat img = cv::imread("/home/osboxes/CLionProjects/wuerfel/img/Domino_black.jpg");

	cv::Mat backgroundFrame = cv::Mat(cv::Size(img.cols, img.rows), CV_8UC3, cv::Scalar(255, 255, 255));
	cvtColor(backgroundFrame, backgroundFrame, CV_BGR2GRAY);

	// will hold our frame
	cv::Mat frame;
	cv::Mat unprocessFrame;

	//for (;;)
	//{
		frame = img;

		// hold unprocessed framed
		unprocessFrame = frame.clone();

		// convert to grayscale
		cvtColor(frame, frame, CV_BGR2GRAY);

		// remove background
		cv::absdiff(frame, backgroundFrame, frame);

		// threshold
		cv::threshold(frame, frame, 150, 255, cv::THRESH_BINARY | CV_THRESH_OTSU);

		// applying canny edge filter
		cv::Canny(frame, frame, 2, 2 * 2, 3, false);

		cv::imshow("frame", frame); cv::waitKey(1);

		// detect dice shapes
		std::vector<std::vector<cv::Point> > diceContours;
		std::vector<cv::Vec4i> diceHierarchy;
		cv::findContours(frame.clone(), diceContours, diceHierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        cv::Mat drawing = cv::Mat::zeros(img.size(), CV_8UC3);
        for(int i=0; i < diceContours.size(); i++) {
            cv::Scalar color = cv::Scalar(150, 0, 32);
            drawContours(drawing, diceContours, i, color, 2, 8, diceHierarchy, 0, cv::Point());

        }
        cv::imshow("drawing", drawing);
        cv::waitKey(0);

		// iterate over dice contours
        for(int i=0; i < diceContours.size(); i++){
            auto diceContour = diceContours.at(i);

            cv::RotatedRect minAreaRotatedRect = cv::minAreaRect(diceContour);
            cv::Rect minAreaRect = minAreaRotatedRect.boundingRect();

            // get contour area
            double diceContourArea = minAreaRect.width * minAreaRect.height;

			std::cout << "area: " << diceContourArea << std::endl;

			// filter contours based on our dice size
			if (diceContourArea > DOMINO_PIECE_AREA_MIN && diceContourArea < DOMINO_PIECE_AREA_MAX) {

				// get bounding rect
				cv::Rect diceBoundsRect = cv::boundingRect(cv::Mat(diceContour));

				// set dice roi
				cv::Mat diceROI = unprocessFrame(diceBoundsRect);

				cv::imshow("rect", diceROI); cv::waitKey(0);

				// count number of pips
				long numberOfPips = countPips(diceROI);

				if (numberOfPips > 0) {

					// output debug info
					std::ostringstream diceText;
					diceText << "val: " << numberOfPips;

                    std::cout << diceText.str() << std::endl;

					// draw value
					cv::putText(unprocessFrame, diceText.str(),
						cv::Point(diceBoundsRect.x, diceBoundsRect.y + diceBoundsRect.height + 20),
						cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar::all(150), 1, 8
					);

					// draw bounding rect
					cv::Scalar color = cv::Scalar(0, 153, 255);
					cv::rectangle(unprocessFrame, diceBoundsRect.tl(), diceBoundsRect.br(), color, 2, 8, 0);

					// show
					cv::imshow("frame", unprocessFrame); cv::waitKey(0);
				}
				else {
					std::cout << "No pips found!" << std::endl;
				}

			}

		}
		// on key press exit
		//if (cv::waitKey(30) >= 0) break;
	//}

	std::cout << "Press any key to exit.";
	std::cin.ignore();
	std::cin.get();

}