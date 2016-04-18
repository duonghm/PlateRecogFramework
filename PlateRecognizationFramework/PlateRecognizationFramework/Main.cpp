#include <iostream>
#include <CameraConvertor.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//#define TEST_CAMERA_CONVERTOR
//#define TEST_PLATE_DETECTOR
//#define TEST_PLATE_EXTRACTOR
//#define TEST_PLATE_RECOGNIZATOR
#define TEST_VLC
#define ENABLE_PLATE_DETECTION
#define ISCROP
//#define CROP_CHARACTER
#define KNN_READER

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
int frameNum = 0;
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

std::vector<cv::Rect> findCharacterRect(cv::Mat& img, cv::Mat& proccessedImg){
	std::vector<cv::Rect> result;

	cv::resize(img, img, img.size()*_scaleFactor);
	cv::Mat customeImgProcess;

	cv::cvtColor(img, customeImgProcess, cv::COLOR_RGB2GRAY);
	cv::equalizeHist(customeImgProcess, customeImgProcess);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::adaptiveThreshold(customeImgProcess, customeImgProcess, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 37 * _scaleFactor, 1);
	cv::imwrite("../cropData/cropCharacter/process_binary_" + std::to_string(frameNum) + ".jpg", customeImgProcess);
	proccessedImg = customeImgProcess.clone();

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
			//cv::Scalar color = cv::Scalar(255, 0, 255);
			//rectangle(img, boundRects[i].tl(), boundRects[i].br(), color, 2, 8, 0);
			result.push_back(boundRects[i]);
			//drawContours(img, contours_polys, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
			//cv::imwrite("../cropData/cropCharacter/char_" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg", img(boundRects[i]));
		}
	}

	customeImgProcess.release();
	contours_polys.clear();
	boundRects.clear();
	//img.release();

	return result;
}

cv::Mat createVirtualPlate(cv::Mat& rawPlate){
	cv::Mat processedImg;
	std::vector<cv::Rect> rect = findCharacterRect(rawPlate, processedImg);

	int maxWidth = 0;
	int maxHeight = 0;
	for (int i = 0; i < rect.size(); i++){
		if (rect[i].width > maxWidth){
			maxWidth = rect[i].width;
		}
		if (rect[i].height > maxHeight){
			maxHeight = rect[i].height;
		}
	}

	cv::Mat virtualPlate = cv::Mat(maxHeight + 10, 10 + (maxWidth)* rect.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	//cv::Mat virtualPlate = cv::Mat(400, 600, CV_8UC3, cv::Scalar(255, 255, 255));
	cv::Mat resultImg;
	cv::cvtColor(processedImg, resultImg, cv::COLOR_GRAY2RGB);
	//std::cout << "Size: " << resultImg.size() << std::endl;	
	for (int i = 0; i < rect.size(); i++){
		//rectangle(resultImg, rect[i].tl(), rect[i].br(), cv::Scalar(255, 255, 0), 2, 8, 0);				
		cv::Mat charImg = resultImg(rect[i]);
		//cv::imwrite("../cropData/virtualPlate/" + std::to_string(frameNum) + "_char_" + std::to_string(i) + ".jpg", charImg);
		//imshow("crop", charImg);
		charImg.copyTo(
			virtualPlate(cv::Rect(5 + i*maxWidth, 5, charImg.cols, charImg.rows))
			);
		charImg.release();		
	}
	//resultImg.release();

	//imshow("Processed Img", resultImg);

	return virtualPlate;
}
#endif // CROP_CHARACTER

#ifdef KNN_READER
#include "KNNTextRecognizer.h"

KNNTextRecognizer knn;
#endif // KNN_READER


#ifdef TEST_VLC
//#include "windows.h"
#include "vlc\vlc.h"
#include "PlateRecognizator.h"

float scale = 1;
#define VIDEO_WIDTH     1920/scale
#define VIDEO_HEIGHT    1080/scale

int cropX = 300 / scale;
int cropY = 300 / scale;
int cropWidth = 1400 / scale;
int cropHeight = 780 / scale;
cv::Rect cropRect = cv::Rect(cropX, cropY, cropWidth, cropHeight);

PlateRecognizator recognizator_rect;
std::vector<pr::PlateRegion> plates_rect;
// define output video resolution
//#define VIDEO_WIDTH     2592/2
//#define VIDEO_HEIGHT    1920/2

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
		//cv::Mat process = dis.clone();		

#ifdef ENABLE_PLATE_DETECTION
		recognizator_rect.SetImg(frame);
		plates_rect = recognizator_rect.GetPlateRegions();
		for (int i = 0; i < plates_rect.size(); i++){
			cv::rectangle(dis, plates_rect[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);
			string filename = "../cropData/cropPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg";
			//cv::imwrite(filename, frame(plates_rect[i].region));

#ifdef CROP_CHARACTER
			//writeCharRect(frame(plates_rect[i].region));
			string virtualPlatefilename = "../cropData/virtualPlate/virtual_" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg";
			cv::Mat virtualPlate = createVirtualPlate(frame(plates_rect[i].region));
			//cv::imwrite(virtualPlatefilename, virtualPlate);
			if (virtualPlate.size().width > 0 && virtualPlate.size().height > 0){
				cv::imshow("virtual plate", virtualPlate);
				cv::imwrite(virtualPlatefilename, virtualPlate);
				//std::cout << recognizator_rect.GetResult(virtualPlate) << std::endl;
				//std::cout << "Plate at frame " << std::to_string(frameNum) << ": " << recognizator_rect.GetResult(virtualPlate) << std::endl;
			}
#endif // CROP_CHARACTER

#ifdef KNN_READER			
			imshow("current plate", frame(plates_rect[i].region));
			std::cout << knn.GetText(frame(plates_rect[i].region)) << std::endl;			
#endif
		}
		

#endif // ENABLE_PLATE_DETECTION
		
		frameNum++;
				
		cv::imshow("frame", dis);
		//cv::imshow("frame", process);

		if (cv::waitKey(1) == 27)
		{
			isRunning = false;
			//exit(0);
		}
		dis.release();
	}	
	frame.release();
	//ReleaseMutex(ctx->mutex);
}

#endif

void processFrame(cv::Mat& frame){
	if (frame.data)
	{
#ifdef ISCROP
		frame = frame(cropRect);
#endif // ISCROP				
		cv::Mat dis = frame.clone();
		cv::Mat process = dis.clone();

#ifdef ENABLE_PLATE_DETECTION
		recognizator_rect.SetImg(frame);		
		plates_rect = recognizator_rect.GetPlateRegions();
		for (int i = 0; i < plates_rect.size(); i++){
			cv::rectangle(dis, plates_rect[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);
			string filename = "../cropData/cropPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg";
			//cv::imwrite(filename, frame(plates_rect[i].region));

#ifdef CROP_CHARACTER
			//writeCharRect(frame(plates_rect[i].region));
			string virtualPlatefilename = "../cropData/virtualPlate/virtual_" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg";
			cv::Mat virtualPlate = createVirtualPlate(frame(plates_rect[i].region));
			//cv::imwrite(virtualPlatefilename, virtualPlate);
			if (virtualPlate.size().width > 0 && virtualPlate.size().height > 0){
				cv::imshow("virtual plate", virtualPlate);
				//cv::imwrite(virtualPlatefilename, virtualPlate);
				//std::cout << recognizator_rect.GetResult(virtualPlate) << std::endl;
				//std::cout << "Plate at frame " << std::to_string(frameNum) << ": " << recognizator_rect.GetResult(virtualPlate) << std::endl;
#ifdef USE_TESSERACT_DIRECT
				api->SetImage((uchar*)virtualPlate.data, virtualPlate.cols, virtualPlate.rows, 1, virtualPlate.cols);
				char* ocrResult = api->GetUTF8Text();
				std::cout << ocrResult << std::endl;
				delete ocrResult;
#endif // USE_TESSERACT_DIRECT
			}
#endif // CROP_CHARACTER
		}

#endif // ENABLE_PLATE_DETECTION

	}
}

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
	CameraConvertor cam = CameraConvertor("F:/114817-vv-1.avi", VIDEO, cv::IMREAD_ANYCOLOR);
	//CameraConvertor cam = CameraConvertor("F:/FIT8/short2.ts", VIDEO, cv::IMREAD_ANYCOLOR);	
	//recognizator_rect;
	recognizator_rect.Init("../data/plateCascade/vn_small.xml");
#ifdef USE_TESSERACT_DIRECT
	api = new tesseract::TessBaseAPI();
	api->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
	api->Init("../data/tessdata/", "leu");
	api->SetVariable("tessedit_char_whitelist", "0123456789");
#endif 
		
	while (true){
		//try{
			cv::Mat img = cam.GetImage();			
			img = img(cropRect);
			cv::Mat dis = img.clone();
			recognizator_rect.SetImg(img);
			plates_rect = recognizator_rect.GetPlateRegions();
			for (int i = 0; i < plates_rect.size(); i++){
				cv::rectangle(dis, plates_rect[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);
				cv::Mat virtualPlate = createVirtualPlate(img(plates_rect[i].region));
				if (virtualPlate.size().width > 0 && virtualPlate.size().height > 0){
					cv::imshow("virtual plate", virtualPlate);
					api->SetImage((uchar*)virtualPlate.data, virtualPlate.cols, virtualPlate.rows, 1, virtualPlate.cols);
					char* ocrResult = api->GetUTF8Text();
					std::cout << ocrResult << std::endl;
					delete ocrResult;
					virtualPlate.release();
				}
			}			
			cv::imshow("FIT8", dis);
			if (cv::waitKey(1) > 0)break;
			img.release();
		//}
		//catch (const std::exception& e){
		//	continue;
		//}
	}
	
#endif
	
#ifdef TEST_VLC
	recognizator_rect.Init("../data/plateCascade/vn_small.xml");

#ifdef KNN_READER
	knn.Init("../data/knndata/classifications.xml", "../data/knndata/images.xml");
#endif // KNN_READER

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
	int vlc_argc = 4;

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
		//Sleep(1);
	}

	libvlc_media_player_stop(mp);
	libvlc_release(vlcInstance);
	libvlc_media_player_release(mp);
	free(context);
#endif

	cv::waitKey();
	return 0;
}