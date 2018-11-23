// std lib
#include <iostream>
#include <sstream>
#include <algorithm>

// OpenCV
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>


static const int DOMINO_PIECE_AREA_MIN = 18000;
static const int DOMINO_PIECE_AREA_MAX = 100353;

// https://stackoverflow.com/questions/43342199/draw-rotated-rectangle-in-opencv-c
void DrawRotatedRectangle(cv::Mat& image, cv::RotatedRect rotatedRect)
{
    cv::Point centerPoint = rotatedRect.center;
    cv::Size rectangleSize = rotatedRect.size;
    double rotationDegrees = rotatedRect.angle;

    cv::Scalar color = cv::Scalar(255.0, 255.0, 255.0); // white

    // Create the rotated rectangle
    cv::RotatedRect rotatedRectangle(centerPoint, rectangleSize, rotationDegrees);

    // We take the edges that OpenCV calculated for us
    cv::Point2f vertices2f[4];
    rotatedRectangle.points(vertices2f);

    // Convert them so we can use them in a fillConvexPoly
    cv::Point vertices[4];
    for(int i = 0; i < 4; ++i){
        vertices[i] = vertices2f[i];

    }

    // Now we can fill the rotated rectangle with our specified color
    cv::fillConvexPoly(image,
                       vertices,
                       4,
                       color);

}



void rotate2D(const cv::Mat & src, cv::Mat & dst, const double degrees)
{
    cv::Point2f center(src.cols/2.0, src.rows/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, degrees, 1.0);
    cv::Rect bbox = cv::RotatedRect(center,src.size(), degrees).boundingRect();

    rot.at<double>(0,2) += bbox.width/2.0 - center.x;
    rot.at<double>(1,2) += bbox.height/2.0 - center.y;

    cv::warpAffine(src, dst, rot, bbox.size());
}

unsigned long countPips(cv::Mat dice) {

	// resize
	cv::resize(dice, dice, cv::Size(150, 150));

	// convert to grayscale
	//cvtColor(dice, dice, CV_BGR2GRAY);

	// threshold
	cv::threshold(dice, dice, 150, 255, cv::THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite("domino_pips_bin.jpg", dice);
	// floodfill
	cv::floodFill(dice, cv::Point(0, 0), cv::Scalar(255));
	cv::floodFill(dice, cv::Point(0, 149), cv::Scalar(255));
	cv::floodFill(dice, cv::Point(149, 0), cv::Scalar(255));
	cv::floodFill(dice, cv::Point(149, 149), cv::Scalar(255));
    cv::imwrite("domino_pips_flood.jpg", dice);
	// show
	cv::imwrite("processed.jpg", dice);

	// search for blobs
	cv::SimpleBlobDetector::Params params;

	cv::Mat diceTemp;
    dice.copyTo(diceTemp);
	cv::GaussianBlur(dice, diceTemp, cv::Size(9,9), 3.3);
	dice = diceTemp;
    cv::imwrite("domino_pips_gauss.jpg", dice);
    // show
    cv::imwrite("blur.jpg", dice);

	// filter by interia defines how elongated a shape is.
	//params.filterByInertia = true;
	//params.minInertiaRatio = 0.5;

    params.filterByColor = true;
    params.blobColor = 0;
    params.minArea = 200;

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

    cv::Mat img = cv::imread("/home/osboxes/CLionProjects/wuerfel/img/tisch_6_rot.jpg");

    cv::Mat imgPervious = cv::imread("/home/osboxes/CLionProjects/wuerfel/img/tisch_5_rot.jpg");
	cvtColor(imgPervious, imgPervious, CV_BGR2GRAY);

	// will hold our frame
	cv::Mat frame;
	cv::Mat unprocessedFrame;
	cv::Mat diffframe;

    // color for drawing into img
    cv::Scalar brightColor = cv::Scalar(255, 0, 242);

    frame = img;

    // hold unprocessed framed
    unprocessedFrame = frame.clone();

    // convert to grayscale
    cvtColor(frame, frame, CV_BGR2GRAY);

    // remove background
    cv::absdiff(frame, imgPervious, frame);
    cv::imwrite("domino_diff.jpg", frame);
    diffframe = frame.clone();

    // threshold
    cv::threshold(frame, frame, 150, 255, cv::THRESH_BINARY | CV_THRESH_OTSU);
    cv::imwrite("domino_bin.jpg", frame);

    cv::Mat tmp = frame.clone();
    cv::blur( tmp, frame, cv::Size(3,3) );
    cv::imwrite("domino_bin_blur.jpg", frame);

    // applying canny edge filter
    cv::Canny(frame, frame, 2, 2 * 2, 3, false);
    cv::imwrite("domino_canny.jpg", frame);

    cv::imwrite("frame.jpg", frame);

    // detect dice shapes
    std::vector<std::vector<cv::Point> > diceContours;
    std::vector<cv::Vec4i> diceHierarchy;
    cv::findContours(frame.clone(), diceContours, diceHierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat drawing = cv::Mat::zeros(img.size(), CV_8UC3);
    for(int i=0; i < diceContours.size(); i++) {
        cv::Scalar color = cv::Scalar(150, 0, 32);
        drawContours(drawing, diceContours, i, color, 2, 8, diceHierarchy, 0, cv::Point());

        cv::Mat singleContour = cv::Mat::zeros(img.size(), CV_8UC3);
        drawContours(singleContour, diceContours, i, color, 2, 8, diceHierarchy, 0, cv::Point());
        std::ostringstream contourName;
        contourName << "domino_contour" <<  i << ".jpg";
        cv::imwrite( contourName.str(), singleContour);
    }
    cv::imwrite("domino_contours.jpg", drawing);

    // iterate over dice contours
    for(int i=0; i < diceContours.size(); i++){
        auto diceContour = diceContours.at(i);

        cv::RotatedRect minAreaRotatedRect = cv::minAreaRect(diceContour);
        cv::Mat rotated = unprocessedFrame.clone();
        DrawRotatedRectangle(rotated, minAreaRotatedRect);

        cv::imwrite("domino_rotated_rect.jpg", rotated);

        cv::Rect diceBoundsRect = minAreaRotatedRect.boundingRect();

        // get contour area
        double diceContourArea = diceBoundsRect.area();

        std::cout << "area: " << diceContourArea << std::endl;

        // filter contours based on our dice size
        if (diceContourArea > DOMINO_PIECE_AREA_MIN && diceContourArea < DOMINO_PIECE_AREA_MAX) {

            // set dice roi
            cv::Mat diceROI = diffframe(diceBoundsRect);

            cv::imwrite("rect.jpg", diceROI);

            // rotate img to zero degree
            cv::Point2f center = cvPoint(diceBoundsRect.height / 2, diceBoundsRect.width / 2);

            cv::Mat upright;

            /*#### Correct rotation angle - Set Upright (+90deg) - Set Horizontal (90deg - angle)### */
            float corrected_angle_deg = minAreaRotatedRect.angle;
            if (minAreaRotatedRect.size.width > minAreaRotatedRect.size.height) {
                corrected_angle_deg = 90 + corrected_angle_deg;
            }
            rotate2D(diceROI, upright, corrected_angle_deg);

            //cv::line(upright, cv::Point(0, upright.rows/2), cv::Point(upright.cols, upright.rows/2), brightColor, 1);
            cv::imwrite("domino_upright.jpg", upright);

            cv::Rect aHalfRect = cv::Rect( cv::Point(0,0), cv::Point(upright.cols, upright.rows/2));
            //cv::rectangle(aHalf, aHalfRect, brightColor, CV_FILLED, 8, 0);
            cv::Mat aHalfImg = upright(aHalfRect);
            cv::imwrite("aHalfImg.jpg", aHalfImg);

            cv::Rect bHalfRect = cv::Rect( cv::Point(0,upright.rows/2), cv::Point(upright.cols, upright.rows));
            //cv::rectangle(aHalf, aHalfRect, brightColor, CV_FILLED, 8, 0);
            cv::Mat bHalfImg = upright(bHalfRect);
            cv::imwrite("bHalfImg.jpg", bHalfImg);

            std::ostringstream pipsResults;
            pipsResults << "Number of pips a,b: ";
            long numberOfPipsA = countPips(aHalfImg);
            long numberOfPipsB = countPips(bHalfImg);
            pipsResults << numberOfPipsA << "," << numberOfPipsB;
            std::cout << pipsResults.str() << std::endl;

            long numberOfAllPips = numberOfPipsA + numberOfPipsB;
            if (numberOfAllPips > 0) {

                // output debug info
                std::ostringstream diceText;
                diceText << "val: " << numberOfPipsA << " + " << numberOfPipsB << " = " <<numberOfAllPips;

                std::cout << diceText.str() << std::endl;

                // draw value
                cv::putText(unprocessedFrame, diceText.str(),
                    cv::Point(diceBoundsRect.x, diceBoundsRect.y + diceBoundsRect.height + 20),
                    cv::FONT_HERSHEY_COMPLEX, 0.8, brightColor, 1, 8
                );

                // draw bounding rect
                //cv::rectangle(unprocessedFrame, diceBoundsRect.tl(), diceBoundsRect.br(), brightColor, 2, 8, 0);
                cv::drawContours(unprocessedFrame, diceContours, 0, brightColor, 2, 8, diceHierarchy);

                // show
                cv::imshow("frame", unprocessedFrame); cv::waitKey(0);
                cv::imwrite("domino_result.jpg", unprocessedFrame);
            }
            else {
                std::cout << "No pips found!" << std::endl;
            }
        }
    }

	std::cout << "Press any key to exit.";
	std::cin.ignore();
	std::cin.get();

}