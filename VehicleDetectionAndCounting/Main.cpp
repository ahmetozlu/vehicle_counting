/*
Author : Ahmet Özlü
Marmara University - CSE Depratment
ahmet.ozlu@marun.edu.tr
*/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#include <fstream>
using namespace std;
#include <string>
#include <iomanip>
#include <ctime>
#pragma warning(disable : 4996)
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows

#include "Blob.h"

#define SHOW_STEPS            // un-comment or comment this line to show steps or not

// global variables ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

// function prototypes ////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs);
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex);
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs);
double distanceBetweenPoints(cv::Point point1, cv::Point point2);
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName);
bool checkIfBlobsCrossedTheLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount);
bool checkIfBlobsCrossedTheLineLeft(std::vector<Blob> &blobs, int &intHorizontalLinePositionLeft, int &carCountLeft);
bool checkIfBlobsCrossedTheLineU(std::vector<Blob> &blobs, int &intHorizontalLinePositionU, int &carCountU);
bool checkIfBlobsCrossedTheLinePedestrian(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &pedestrianCount);
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy);
void drawCarCountOnImage(int &carCount, cv::Mat &imgFrame2Copy);


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int year;
int month;
int day;
int nowIs;
int saat;
int dakika;
int saniye;
std::stringstream date;
int fileNameControl = 0;
int fileNameControlRight = 0;
int fileNameControlU = 0;
int carCountLeft = 0;
int carCountU = 0;
int pedestrianCount = 0;
int intVerticalLinePosition;
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {


	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	time_t nowW = time(NULL);
	struct tm nowLocal;
	nowLocal = *localtime(&nowW);
	saat = nowLocal.tm_hour;
	dakika = nowLocal.tm_min;
	saniye = nowLocal.tm_sec;
	time_t now = time(0);
	tm *ltm = localtime(&now);
	
	date << ltm->tm_mday
		<< "/"
		<< 1 + ltm->tm_mon
		<< "/"
		<< 1900 + ltm->tm_year
		<< " "
		<< 1 + ltm->tm_hour
		<< ":"
		<< 1 + ltm->tm_min
		<< ":"
		<< 1 + ltm->tm_sec;
 
	//get the starting value of clock
	tm* my_time;

	//get current time in format of time_t
	time_t t = time(NULL);
	 
	//convert time_t to tm
	my_time = localtime(&t);

	//get only hours and minutes
	char* hhMM = new char[6];
	strftime(hhMM, 6, "HH:MM", my_time);

	year = 1900 + my_time->tm_year;
	month = my_time->tm_mon + 1;
	day = ltm->tm_mday;
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------


    cv::VideoCapture capVideo;

    cv::Mat imgFrame1;
    cv::Mat imgFrame2;

    std::vector<Blob> blobs;
	
    cv::Point crossingLine[2];
	cv::Point crossingLineLeft[2];
	cv::Point crossingLineMiddle[2];
	cv::Point crossingLinePedestrian[2];

    int carCount = 0;
	
	
    capVideo.open("HSCC Interstate Highway Surveillance System - YouTube_4 (online-video-cutter.com).mp4");

    if (!capVideo.isOpened()) {                                                 // if unable to open video file
        std::cout << "error reading video file" << std::endl << std::endl;      // show error message
        _getch();                   // it may be necessary to change or remove this line if not using Windows
        return(0);                                                              // and exit program
    }

    if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 2) {
        std::cout << "error: video file must have at least two frames";
        _getch();                   // it may be necessary to change or remove this line if not using Windows
        return(0);
    }

    capVideo.read(imgFrame1);
    capVideo.read(imgFrame2);


	//CONTROL LINE FOR CARCOUNT ~AREA1(RIGHT WAY)
	int intHorizontalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35);
	intHorizontalLinePosition = intHorizontalLinePosition*1.40;
	intVerticalLinePosition = (int)std::round((double)imgFrame1.cols * 0.35);

	crossingLine[0].x = 515;
	crossingLine[0].y = intHorizontalLinePosition;

	crossingLine[1].x = imgFrame1.cols - 1;
	crossingLine[1].y = intHorizontalLinePosition;

	//CONTROL LINE FOR CARCOUNT ~AREA2(LEFT WAY)
	crossingLineLeft[0].x = 0;
	crossingLineLeft[0].y = intHorizontalLinePosition;

	crossingLineLeft[1].x = 300;
	crossingLineLeft[1].y = intHorizontalLinePosition;

    char chCheckForEscKey = 0;

    bool blnFirstFrame = true;

    int frameCount = 2;

    while (capVideo.isOpened() && chCheckForEscKey != 27) {

        std::vector<Blob> currentFrameBlobs;

        cv::Mat imgFrame1Copy = imgFrame1.clone();
        cv::Mat imgFrame2Copy = imgFrame2.clone();

        cv::Mat imgDifference;
        cv::Mat imgThresh;

        cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
        cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

        cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
        cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);

        cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

        cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);

        cv::imshow("imgThresh", imgThresh);

        cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
        cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

        for (unsigned int i = 0; i < 2; i++) {
            cv::dilate(imgThresh, imgThresh, structuringElement5x5);
            cv::dilate(imgThresh, imgThresh, structuringElement5x5);
            cv::erode(imgThresh, imgThresh, structuringElement5x5);
        }

        cv::Mat imgThreshCopy = imgThresh.clone();

        std::vector<std::vector<cv::Point> > contours;

        cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        drawAndShowContours(imgThresh.size(), contours, "imgContours");

        std::vector<std::vector<cv::Point> > convexHulls(contours.size());

        for (unsigned int i = 0; i < contours.size(); i++) {
            cv::convexHull(contours[i], convexHulls[i]);
        }

        drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

        for (auto &convexHull : convexHulls) {
            Blob possibleBlob(convexHull);

            if (possibleBlob.currentBoundingRect.area() > 400 &&
                possibleBlob.dblCurrentAspectRatio > 0.2 &&
                possibleBlob.dblCurrentAspectRatio < 4.0 &&
                possibleBlob.currentBoundingRect.width > 30 &&
                possibleBlob.currentBoundingRect.height > 30 &&
                possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                (cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50) {
                currentFrameBlobs.push_back(possibleBlob);
            }
        }

        drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

        if (blnFirstFrame == true) {
            for (auto &currentFrameBlob : currentFrameBlobs) {
                blobs.push_back(currentFrameBlob);
            }
        } else {
            matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
        }

        drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

        imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

        drawBlobInfoOnImage(blobs, imgFrame2Copy);

		// Check the leftWay
		bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, intHorizontalLinePosition, carCount);
		// Check the rightWay
		bool blnAtLeastOneBlobCrossedTheLineLeft = checkIfBlobsCrossedTheLineLeft(blobs, intHorizontalLinePosition, carCountLeft);
		
		//leftway
        if (blnAtLeastOneBlobCrossedTheLine == true) {
            cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);			
        }
		else if (blnAtLeastOneBlobCrossedTheLine == false) {
            cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);			
        }

		//rightWay
		if (blnAtLeastOneBlobCrossedTheLineLeft == true) {
			cv::line(imgFrame2Copy, crossingLineLeft[0], crossingLineLeft[1], SCALAR_WHITE, 2);
		}
		else if (blnAtLeastOneBlobCrossedTheLineLeft == false){
			cv::line(imgFrame2Copy, crossingLineLeft[0], crossingLineLeft[1], SCALAR_YELLOW, 2);
		}


		drawCarCountOnImage(carCount, imgFrame2Copy);

        cv::imshow("imgFrame2Copy", imgFrame2Copy);

        //cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging DEBUG ~a47
        
        // now we prepare for the next iteration

        currentFrameBlobs.clear();

        imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is

        if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT)) {
            capVideo.read(imgFrame2);
        }
        else {
            std::cout << "end of video\n";
            break;
        }

        blnFirstFrame = false;
        frameCount++;
        chCheckForEscKey = cv::waitKey(1);
    }

    if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
        cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
    }
    // note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows

    return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs) {

    for (auto &existingBlob : existingBlobs) {

        existingBlob.blnCurrentMatchFoundOrNewBlob = false;

        existingBlob.predictNextPosition();
    }

    for (auto &currentFrameBlob : currentFrameBlobs) {

        int intIndexOfLeastDistance = 0;
        double dblLeastDistance = 100000.0;

        for (unsigned int i = 0; i < existingBlobs.size(); i++) {

            if (existingBlobs[i].blnStillBeingTracked == true) {

                double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

                if (dblDistance < dblLeastDistance) {
                    dblLeastDistance = dblDistance;
                    intIndexOfLeastDistance = i;
                }
            }
        }

        if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 0.5) {
            addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
        }
        else {
            addNewBlob(currentFrameBlob, existingBlobs);
        }

    }

    for (auto &existingBlob : existingBlobs) {

        if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
            existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
        }

        if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5) {
            existingBlob.blnStillBeingTracked = false;
        }

    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex) {

    existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
    existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;

    existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());

    existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
    existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;

    existingBlobs[intIndex].blnStillBeingTracked = true;
    existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs) {

    currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;

    existingBlobs.push_back(currentFrameBlob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double distanceBetweenPoints(cv::Point point1, cv::Point point2) {
    
    int intX = abs(point1.x - point2.x);
    int intY = abs(point1.y - point2.y);

    return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

    cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName) {
    
    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

    std::vector<std::vector<cv::Point> > contours;

    for (auto &blob : blobs) {
        if (blob.blnStillBeingTracked == true) {
            contours.push_back(blob.currentContour);
        }
    }

    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

    cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedTheLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount) {
    bool blnAtLeastOneBlobCrossedTheLine = false;

    for (auto blob : blobs) {

        if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
            int prevFrameIndex = (int)blob.centerPositions.size() - 2;
            int currFrameIndex = (int)blob.centerPositions.size() - 1;

			// Left way
			if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition && blob.centerPositions[currFrameIndex].x > 350) {
                carCount++;					
				

				//----------------------------------------------------------------------------
				//----------------------------------------------------------------------------
				string firstStringGroupOfSentence = "{\"date\":";
				string path = "C:\\Users\\Ahmet-Erarge\\Desktop\\carCounting\\OpenCV_OpenGL\\OpenCV_OpenGL\\leftWay\\";
				string amOrPm;
				
				// detect AM or PM
				if (saat > 12) {
					amOrPm = "PM";
				}
				else {
					amOrPm = "AM";
				}
	
				// converting int to strings (hours, month etc.)
				std::string s_carCount = std::to_string(carCount);
				std::string s_year = std::to_string(year);
				std::string s_month = std::to_string(month);
				std::string s_day = std::to_string(day);
				std::string s_saat = std::to_string(saat);
				std::string s_dakika = std::to_string(dakika);
				std::string s_saniye = std::to_string(saniye);

				// set the sentence
				string total = firstStringGroupOfSentence + s_day + "/" + s_month + "/" + s_year + " " + s_saat + ":" + s_dakika + ":" + s_saniye + " " + amOrPm + "\",\"" + "routes" + "\"" + ":[" + s_carCount + "]}";
				
				fstream myfile;
				// set the file name
				string fileName = path + s_year + "-" + s_day + "-" + s_month + "_" + s_saat + "-" + s_dakika + "-" + s_saniye + ".txt";
				
				if (fileNameControl == 0) {
					fileNameControl++;
					std::ofstream of(fileName);
				}

				myfile.open(fileName);
				myfile << total;
				myfile.close();
				//----------------------------------------------------------------------------
				//----------------------------------------------------------------------------


                blnAtLeastOneBlobCrossedTheLine = true;
            }

        }

    }

    return blnAtLeastOneBlobCrossedTheLine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedTheLineLeft(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCountLeft) {
	bool blnAtLeastOneBlobCrossedTheLineLeft = false;

	for (auto blob : blobs) {

		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;

			// Left way
			if (blob.centerPositions[prevFrameIndex].y <= intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].x < 350 && blob.centerPositions[currFrameIndex].x > 0) {
				carCountLeft++;


				//----------------------------------------------------------------------------
				//----------------------------------------------------------------------------
				string firstStringGroupOfSentence = "{\"date\":";
				string path = "C:\\Users\\Ahmet-Erarge\\Desktop\\carCounting\\OpenCV_OpenGL\\OpenCV_OpenGL\\rightWay\\";
				string amOrPm;

				// detect AM or PM
				if (saat > 12) {
					amOrPm = "PM";
				}
				else {
					amOrPm = "AM";
				}

				// converting int to strings (hours, month etc.)
				std::string s_carCountLeft = std::to_string(carCountLeft);
				std::string s_year = std::to_string(year);
				std::string s_month = std::to_string(month);
				std::string s_day = std::to_string(day);
				std::string s_saat = std::to_string(saat);
				std::string s_dakika = std::to_string(dakika);
				std::string s_saniye = std::to_string(saniye);

				// set the sentence
				string total = firstStringGroupOfSentence + s_day + "/" + s_month + "/" + s_year + " " + s_saat + ":" + s_dakika + ":" + s_saniye + " " + amOrPm + "\",\"" + "routes" + "\"" + ":[" + s_carCountLeft + "]}";

				fstream myfile;
				// set the file name
				string fileName = path + s_year + "-" + s_day + "-" + s_month + "_" + s_saat + "-" + s_dakika + "-" + s_saniye + ".txt";

				if (fileNameControlRight == 0) {
					fileNameControlRight++;
					std::ofstream of(fileName);
				}

				myfile.open(fileName);
				myfile << total;
				myfile.close();
				//----------------------------------------------------------------------------
				//----------------------------------------------------------------------------


				blnAtLeastOneBlobCrossedTheLineLeft = true;
			}
		}

	}

	return blnAtLeastOneBlobCrossedTheLineLeft;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy) {

    for (unsigned int i = 0; i < blobs.size(); i++) {

        if (blobs[i].blnStillBeingTracked == true) {
            cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);

            int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
			double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
            int intFontThickness = (int)std::round(dblFontScale * 1.0);

            cv::putText(imgFrame2Copy, std::to_string(i), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawCarCountOnImage(int &carCount, cv::Mat &imgFrame2Copy) {

    int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
    double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 450000.0;
    int intFontThickness = (int)std::round(dblFontScale * 2.5);
	
	// Left way
	cv::Size textSize = cv::getTextSize(std::to_string(carCount), intFontFace, dblFontScale, intFontThickness, 0);

	cv::putText(imgFrame2Copy, "Vehicle count:" + std::to_string(carCount), cv::Point(568,25), intFontFace, dblFontScale, SCALAR_RED, intFontThickness);

	// Right way
	cv::Size textSize1 = cv::getTextSize(std::to_string(carCountLeft), intFontFace, dblFontScale, intFontThickness, 0);

	cv::putText(imgFrame2Copy, "Vehicle count:" + std::to_string(carCountLeft), cv::Point(10, 25), intFontFace, dblFontScale, SCALAR_YELLOW, intFontThickness);

}



	






