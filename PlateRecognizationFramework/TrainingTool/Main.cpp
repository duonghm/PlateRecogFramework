#include "opencv2/opencv.hpp"
#include "CameraConvertor.h"
#include <thread>

using namespace std;
using namespace cv;
using namespace pr;

void onMouse(int event, int x, int y, int f, void*);

Rect cropRect(0, 0, 0, 0);
Point P1;
Point P2;
bool clicked = false;
bool isCrop = false;

int currentFrameNumber = 0;
int cropFrameNumber = 0;
int currentFramePos = 0;
int currentFrameNeg = 0;

int main(){
	string fileName = "";
	string positiveFolder = "../cropData/pos/";
	string negativeFolder = "../cropData/neg/";
	string defaultPosFilename = "pos";
	string defaultNegFilename = "neg";
	string defeultExtesion = ".jpg";

	string videoWindowName = "FIT8";	
	namedWindow(videoWindowName);		
	setMouseCallback(videoWindowName, onMouse, NULL);

	//CameraConvertor cam = CameraConvertor("F:/FIT8/fit8.ts", VIDEO, IMREAD_ANYCOLOR);
	CameraConvertor cam = CameraConvertor("F:/FIT8/short4.ts", VIDEO, IMREAD_ANYCOLOR);
	Mat img;
	Mat cropImg;
	Mat displayImg;
	while (true){
		currentFrameNumber++;
		img = cam.GetImage();
		if (img.empty()) continue;
		resize(img, img, img.size() / 4);
		
		if (!isCrop){			
			displayImg = img.clone();
			rectangle(displayImg, cropRect, Scalar(0, 255, 0), 2, 8, 0);
		}
		else{			
			displayImg = cropImg.clone();
			rectangle(displayImg, cropRect, Scalar(0, 255, 0), 2, 8, 0);			
		}
		imshow(videoWindowName, displayImg);
			

		int k = cvWaitKey(33);		
		if (k == -1){
			continue;
		}
		else if(k == 27){
			// ESC key : EXIT
			break;
		}
		else if (k == 32){
			// SPACE key : PAUSE
			std::cout << "Press space key" << std::endl;
			isCrop = !isCrop;
			if (isCrop){
				cropImg = img.clone();
				cropFrameNumber = currentFrameNumber;
			}				
			continue;			
		}
		else if (k == 13){
			// ENTER key
			
		}	
		else if (k == 43){
			// + key : SAVE POSITIVE
			if (cropRect.width > 0 && cropRect.height > 0){
				fileName = positiveFolder + defaultPosFilename + to_string(cropFrameNumber) + "_" + to_string(currentFramePos++) + defeultExtesion;
				cout << fileName << endl;
				Mat roi = cropImg(cropRect);
				imshow("crop", roi);
				imwrite(fileName, roi);
				cout << "=== SAVE POSITIVE CROP IMAGE===" << endl;
			}
		}
		else if (k == 45){
			// - key : SAVE NEGATIVE
			if (cropRect.width > 0 && cropRect.height > 0){
				fileName = negativeFolder + defaultPosFilename + to_string(cropFrameNumber) + "_" + to_string(currentFramePos++) + defeultExtesion;
				cout << fileName << endl;
				Mat roi = cropImg(cropRect);
				imshow("crop", roi);				
				imwrite(fileName, roi);
				cout << "=== SAVE NEGATIVE CROP IMAGE===" << endl;
			}
		}
		else{
			cout << k << endl;
		}
				
	}

	//waitKey(0);
	return 0;

}

void onMouse(int event, int x, int y, int f, void*){
	//cout << event << " " << x << " " << y << endl;
	switch (event){
	case CV_EVENT_LBUTTONDOWN:
		clicked = true;
		P1.x = x;
		P1.y = y;
		P2.x = x;
		P2.y = y;
		break;
	case CV_EVENT_LBUTTONUP:
		P2.x = x;
		P2.y = y;
		clicked = false;
		break;
	case CV_EVENT_MOUSEMOVE:
		if (clicked){
			P2.x = x;
			P2.y = y;
		}
		break;
	case CV_EVENT_RBUTTONUP:
		clicked = false;
		cropRect = Rect(0, 0, 0, 0);
		break;
	default:   
		break;
	}

	if (clicked){
		if (P1.x > P2.x){
			cropRect.x = P2.x;
			cropRect.width = P1.x - P2.x;
		}
		else {
			cropRect.x = P1.x;
			cropRect.width = P2.x - P1.x;
		}

		if (P1.y > P2.y){
			cropRect.y = P2.y;
			cropRect.height = P1.y - P2.y;
		}
		else {
			cropRect.y = P1.y;
			cropRect.height = P2.y - P1.y;
		}
	}

}