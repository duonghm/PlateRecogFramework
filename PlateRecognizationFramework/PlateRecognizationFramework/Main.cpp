#include <iostream>
#include <CameraConvertor.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//#define TEST_CAMERA_CONVERTOR
//#define TEST_PLATE_DETECTOR
//#define TEST_PLATE_EXTRACTOR
//#define TEST_PLATE_RECOGNIZATOR
#define TEST_VLC
//#define TEST_BACKGROUND_SUBTRACTION
#define ENABLE_PLATE_DETECTION
#define ISCROP
#define CROP_CHARACTER

#ifdef TEST_CAMERA_CONVERTOR
#include "TestCameraConvertorModule.h"
#endif 

#ifdef TEST_PLATE_DETECTOR
#include "TestPlateDetectorModule.h"
#endif

#ifdef TEST_PLATE_EXTRACTOR
#include "TestPlateExtractorModule.h"
#endif

#ifdef TEST_PLATE_RECOGNIZATOR
#include "PlateRecognizator.h"
#endif

//using namespace cv;
using namespace pr;
using namespace std;




#ifdef TEST_VLC
#include "windows.h"
#include "vlc\vlc.h"
#include "PlateRecognizator.h"

// define output video resolution
//#define VIDEO_WIDTH     2592/2
//#define VIDEO_HEIGHT    1920/2
float scale = 1;
#define VIDEO_WIDTH     1920/scale
#define VIDEO_HEIGHT    1090/scale

int cropX = 300 / scale;
int cropY = 300 / scale;
int cropWidth = 1400 / scale;
int cropHeight = 790 / scale;
cv::Rect cropRect = cv::Rect(cropX, cropY, cropWidth, cropHeight);


int frameNum= 0;
PlateRecognizator recognizator_rect;
std::vector<pr::PlateRegion> plates_rect;

#ifdef CROP_CHARACTER
int _scaleFactor = 3;
int _minCharWidth = 2 * _scaleFactor;
int _minCharHeight = 10 * _scaleFactor;
int _maxCharWidth = 40 * _scaleFactor;
int _maxCharHeight = 40 * _scaleFactor;
std::vector<std::vector<cv::Point> > contours_polys;
std::vector<cv::Rect> boundRects;

bool isValidCharBox(cv::Rect rect){
	return rect.width > _minCharWidth && rect.height > _minCharHeight && rect.width < _maxCharWidth && rect.height < _maxCharHeight;
}
void writeCharRect(cv::Mat img){
	cv::resize(img, img, img.size()*_scaleFactor);
	//cv::Mat customeImgProcess = img.clone();			
	cv::Mat customeImgProcess;
	cv::cvtColor(img, customeImgProcess, cv::COLOR_RGB2GRAY);
	cv::equalizeHist(customeImgProcess, customeImgProcess);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::adaptiveThreshold(customeImgProcess, customeImgProcess, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37 * _scaleFactor, 1);
	cv::imwrite("../cropData/cropCharacter/process_binary_" + std::to_string(frameNum) + ".jpg", customeImgProcess);
	int erosion_size = 1;
	cv::Mat element = cv::getStructuringElement(
		cv::MORPH_RECT,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));
	cv::dilate(customeImgProcess, customeImgProcess, element);
	
	cv::findContours(customeImgProcess, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	contours_polys = std::vector<std::vector<cv::Point> >(contours.size());
	boundRects = std::vector<cv::Rect>(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contours[i]), contours_polys[i], 1, true);
		boundRects[i] = cv::boundingRect(cv::Mat(contours_polys[i]));
		if (isValidCharBox(boundRects[i])){
			cv::Scalar color = cv::Scalar(255, 0, 255);
			rectangle(img, boundRects[i].tl(), boundRects[i].br(), color, 2, 8, 0);
			//drawContours(img, contours_polys, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
			//cv::imwrite("../cropData/cropCharacter/char_" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg", img(boundRects[i]));
		}		
	}
	cv::imwrite("../cropData/cropCharacter/process_" + std::to_string(frameNum) + ".jpg", img);

	/*for (int i = 0; i < boundRects.size(); i++){
		if (isValidCharBox(boundRects[i])){
			cv::Mat cropChar = img(boundRects[i]);
			equalizeHist(cropChar, cropChar);
			cv::adaptiveThreshold(cropChar, cropChar, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37 * _scaleFactor, 1);
			cv::imwrite("../cropData/cropCharacter/char_" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg", cropChar);
		}
	}*/
}
#endif // CROP_CHARACTER

#ifdef TEST_BACKGROUND_SUBTRACTION
int erosion_size = 7;
cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorMOG2();
cv::Mat element = cv::getStructuringElement(
	cv::MORPH_RECT,
	cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
	cv::Point(erosion_size, erosion_size));

std::vector<std::vector<cv::Point> > contours;
std::vector<cv::Vec4i> hierarchy;
std::vector<std::vector<cv::Point> > contours_poly;
std::vector<cv::Rect> boundRect;
#endif // TEST_BACKGROUND_SUBTRACTION



struct ctx
{
	cv::Mat* image;
	//HANDLE mutex;
	uchar* pixels;
};
bool isRunning = true;

void *lock(void *data, void**p_pixels)
{
	struct ctx *ctx = (struct ctx*)data;
	//WaitForSingleObject(ctx->mutex, INFINITE);
	*p_pixels = ctx->pixels;
	return NULL;

}

void display(void *data, void *id){
	(void)data;	
	assert(id == NULL);
}

void unlock(void *data, void *id, void *const *p_pixels)
{
	struct ctx *ctx = (struct ctx*)data;
	cv::Mat frame = *ctx->image;
	if (frame.data)
	{		
#ifdef ISCROP
		frame = frame(cropRect);
#endif // ISCROP				
		cv::Mat dis = frame.clone();
		cv::Mat process = dis.clone();		
		
#ifdef TEST_BACKGROUND_SUBTRACTION
		pMOG2->apply(frame, process);
		cv::dilate(process, process, element);
		cv::findContours(process, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		contours_poly = std::vector<std::vector<cv::Point>>(contours.size());
		boundRect = std::vector<cv::Rect>(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);	
			boundRect[i] = cv::boundingRect(cv::Mat(contours_poly[i]));
			if (boundRect[i].width > 200 && boundRect[i].height > 200){
				cv::Scalar color = cv::Scalar(255, 0, 255);
				//rectangle(dis, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
			}
		}
#endif // TEST_BACKGROUND_SUBTRACTION

#ifdef ENABLE_PLATE_DETECTION
#ifndef TEST_BACKGROUND_SUBTRACTION
		recognizator_rect.SetImg(frame);
		plates_rect = recognizator_rect.GetPlateRegions();
		for (int i = 0; i < plates_rect.size(); i++){
			cv::rectangle(dis, plates_rect[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);
			string filename = "../cropData/cropPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg";
			cv::imwrite(filename, frame(plates_rect[i].region));

#ifdef CROP_CHARACTER
			writeCharRect(frame(plates_rect[i].region));
#endif // CROP_CHARACTER

		}
#endif // TEST_BACKGROUND_SUBTRACTION
#ifdef TEST_BACKGROUND_SUBTRACTION
		for (int i = 0; i < boundRect.size(); i++){
			recognizator_rect.SetImg(frame(boundRect[i]));
			plates_rect = recognizator_rect.GetPlateRegions();
			for (int i = 0; i < plates_rect.size(); i++){
				cv::rectangle(dis, plates_rect[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);
				string filename = "../cropData/cropPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg";
				//cv::imwrite(filename, frame(plates_rect[i].region));
			}
		}
#endif // TEST_BACKGROUND_SUBTRACTION

#endif // ENABLE_PLATE_DETECTION
		
		frameNum++;
				
		cv::imshow("frame", dis);
		//cv::imshow("frame", process);

		if (cv::waitKey(1) == 27)
		{
			isRunning = false;
			//exit(0);
		}
	}	
	//ReleaseMutex(ctx->mutex);
}



#endif

int main(){
	
#ifdef TEST_CAMERA_CONVERTOR
	Test_CameraModule();
#endif 

#ifdef TEST_PLATE_DETECTOR
	Test_PlateDetectorModule();
#endif

#ifdef TEST_PLATE_EXTRACTOR
	Test_PlateExtractorModule();
#endif

#ifdef TEST_PLATE_RECOGNIZATOR
	CameraConvertor cam = CameraConvertor("F:/FIT8/fit8.ts", VIDEO, cv::IMREAD_ANYCOLOR);
	//CameraConvertor cam = CameraConvertor("F:/FIT8/short2.ts", VIDEO, cv::IMREAD_ANYCOLOR);
	PlateRecognizator recognizator_wide;
	recognizator_wide.Init("../data/plateCascade/eu.xml");
	PlateRecognizator recognizator_rect;
	recognizator_rect.Init("../data/plateCascade/vn_2.xml");

	cv::Mat dis;

	std::vector<pr::PlateRegion> plates_wide;
	std::vector<pr::PlateRegion> plates_rect;
	while (true){
		try{
			cv::Mat img = cam.GetImage();
			cv::resize(img, img, img.size() / 2);
			//std::cout << img.size() << endl;
			dis = img.clone();
			if (img.empty()){ continue; }
			recognizator_wide.SetImg(img);
			recognizator_rect.SetImg(img);
			plates_wide = recognizator_wide.GetPlateRegions();
			plates_rect = recognizator_rect.GetPlateRegions();
			if (plates_wide.size() > 0) std::cout << "Wide: " << plates_wide.size() << std::endl;
			if (plates_rect.size() > 0) std::cout << "Rect: " << plates_rect.size() << std::endl;
			std::cout << "---------" << std::endl;
			for (int i = 0; i < plates_wide.size(); i++){
				cv::rectangle(dis, plates_wide[i].region, cv::Scalar(255, 255, 0), 2, 8, 0);
			}
			for (int i = 0; i<plates_rect.size(); i++){
				cv::rectangle(dis, plates_rect[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);
			}
			//cv::resize(dis, dis, dis.size() / 2);
			cv::imshow("FIT8", dis);
			if (cv::waitKey(1) > 0)break;
		}
		catch (const std::exception& e){
			continue;
		}
		
		
	}
	/*std::vector<std::string> result = recognizator.GetResult();
	for (int i = 0; i < result.size(); i++){
		std::cout << "Plate " << std::to_string(i) << ": " << result[i] << std::endl;
	}*/
#endif
	
#ifdef TEST_VLC
	recognizator_rect.Init("../data/plateCascade/vn_small.xml");

	string url = "F:\\114817-vv-1.avi";
	//vlc sdk does not know the video size until it is rendered, so need to play it a bit so that size is     known
	cv::Size sz = cv::Size(VIDEO_WIDTH, VIDEO_HEIGHT);

	// VLC pointers
	libvlc_instance_t *vlcInstance;
	libvlc_media_player_t *mp;
	libvlc_media_t *media;

	const char * const vlc_args[] = {
		"-I", "dummy", // Don't use any interface
		"--ignore-config", // Don't use VLC's config
		"--extraintf=logger", // Log anything
		"--verbose=2", // Be much more verbose then normal for debugging purpose
	};	
	int vlc_argc = 5;

	vlcInstance = libvlc_new(vlc_argc, vlc_args);

	//for (int i = 0; i < vlc_argc; i++)
	//	delete[] vlc_args[i];

	media = libvlc_media_new_path(vlcInstance, url.c_str());
	mp = libvlc_media_player_new_from_media(media);
	libvlc_media_release(media);
	
	struct ctx* context = (struct ctx*)malloc(sizeof(*context));
	//context->mutex = CreateMutex(NULL, FALSE, NULL);
	context->image = new cv::Mat(sz.height, sz.width, CV_8UC3);
	context->pixels = (unsigned char *)context->image->data;

	libvlc_video_set_callbacks(mp, lock, unlock, display, context);
	libvlc_video_set_format(mp, "RV24", sz.width, sz.height, sz.width * 24 / 8); // pitch = width *     BitsPerPixel / 8
	//libvlc_video_set_format(mp, "RV32", sz.width, sz.height, sz.width * 4);

	libvlc_media_player_play(mp);
	while (isRunning)
	{
		//imshow("rtsp", *(context->image));
		Sleep(1);
	}

	libvlc_media_player_stop(mp);
	libvlc_release(vlcInstance);
	libvlc_media_player_release(mp);
	free(context);
#endif

	cv::waitKey();
	return 0;
}