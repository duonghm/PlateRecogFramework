#include <iostream>
#include <CameraConvertor.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream> 

//#define TEST_CAMERA_CONVERTOR
//#define TEST_PLATE_DETECTOR
//#define TEST_PLATE_EXTRACTOR
//#define TEST_PLATE_RECOGNIZATOR
#define TEST_VLC
#define ENABLE_PLATE_DETECTION
#define ISCROP
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

#ifdef KNN_READER
#include "KNNTextRecognizer.h"

KNNTextRecognizer knn;
std::ofstream ofs;
#endif // KNN_READER

#ifdef TEST_VLC
//#include "windows.h"
#include "vlc\vlc.h"
#include "PlateRecognizator.h"

float scale = 2;
#define VIDEO_WIDTH     1920/scale
#define VIDEO_HEIGHT    1080/scale
//#define VIDEO_WIDTH     2592/scale
//#define VIDEO_HEIGHT    1920/scale

int cropX = 300 / scale;
int cropY = 300 / scale;
int cropWidth = 1400 / scale;
int cropHeight = 780 / scale;
cv::Rect cropRect = cv::Rect(cropX, cropY, cropWidth, cropHeight);

PlateRecognizator recognizator_motor_rect;
PlateRecognizator recognizator_wide;
std::vector<pr::PlateRegion> plates_rect;
std::vector<pr::PlateRegion> plates_wide;
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
int plate_num = 0;

void *lock(void *data, void**p_pixels)
{
	struct ctx *ctx = (struct ctx*)data;
	//WaitForSingleObject(ctx->mutex, INFINITE);
	*p_pixels = ctx->pixels;
	return NULL;

}

void display(void *data, void *id){
	//(void)data;	
	assert(id == NULL);
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
		recognizator_motor_rect.SetImg(frame);
		recognizator_wide.SetImg(frame);
		plates_rect = recognizator_motor_rect.GetPlateRegions();
		plates_wide = recognizator_wide.GetPlateRegions();

		for (int i = 0; i < plates_rect.size(); i++){
			cv::rectangle(dis, plates_rect[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);
			//string filename = "../cropData/cropPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg";
			string filename = "../cropData/cropPlate/" + std::to_string(plate_num++) + ".jpg";
			cv::imwrite(filename, frame(plates_rect[i].region));
			//imshow("Plate", frame(plates_rect[i].region));

#ifdef KNN_READER
			cv::Mat plate = frame(plates_rect[i].region);
			imshow("current plate", plate);
			//std::string plateValue = knn.GetText(frame(plates_rect[i].region));
			cv::Mat proccessedBin;
			std::string plateValue = knn.GetTextDebug(plate, proccessedBin);
			std::cout << plateValue << std::endl;
			ofs << filename << "\t" << plateValue << std::endl;

			cv::imwrite("../cropData/binaryPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg", proccessedBin);
			//cv::imwrite("../cropData/segmentPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg", plate);

			proccessedBin.release();
			plate.release();
#endif
		}

		for (int i = 0; i < plates_wide.size(); i++){
			cv::rectangle(dis, plates_wide[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);			
			string filename = "../cropData/cropPlate/" + std::to_string(plate_num++) + ".jpg";	
			cv::imwrite(filename, frame(plates_wide[i].region));
#ifdef KNN_READER
			cv::Mat plate = frame(plates_wide[i].region);
			imshow("current plate", plate);
			//std::string plateValue = knn.GetText(frame(plates_rect[i].region));
			cv::Mat proccessedBin;
			std::string plateValue = knn.GetTextDebug(plate, proccessedBin);
			std::cout << plateValue << std::endl;
			ofs << filename << "\t" << plateValue << std::endl;

			cv::imwrite("../cropData/binaryPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg", proccessedBin);
			//cv::imwrite("../cropData/segmentPlate/" + std::to_string(frameNum) + "_" + std::to_string(i) + ".jpg", plate);

			proccessedBin.release();
			plate.release();
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
}

void unlock(void *data, void *id, void *const *p_pixels)
{
	
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
	CameraConvertor cam = CameraConvertor("F:/114817-vv-1.avi", VIDEO, cv::IMREAD_ANYCOLOR);
	//CameraConvertor cam = CameraConvertor("F:/FIT8/short2.ts", VIDEO, cv::IMREAD_ANYCOLOR);	
	//recognizator_rect;
	recognizator_motor_rect.Init("../data/plateCascade/vn_small.xml");
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
			recognizator_motor_rect.SetImg(img);
			plates_rect = recognizator_motor_rect.GetPlateRegions();
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
	//recognizator_motor_rect.Init("../data/plateCascade/vn_car_wide.xml", cv::Size(80, 20), cv::Size(200, 50), 2, 3);
	recognizator_motor_rect.Init("../data/plateCascade/vn_rect.xml", cv::Size(30, 30), cv::Size(90, 90), 1.5, 3);
	recognizator_wide.Init("../data/plateCascade/vn_car_wide.xml", cv::Size(80, 20), cv::Size(200, 50), 2, 3);
	//recognizator_motor_rect.Init("../data/plateCascade/vn_rect.xml");

#ifdef KNN_READER
	knn.Init("../data/knndata/classifications.xml", "../data/knndata/images.xml");
	ofs.open("../cropData/result.txt", std::ofstream::out | std::ofstream::app);
#endif // KNN_READER

	//string url = "F:\\114817-vv-1.avi";
	string url = "F:\\wide_plate_test.mp4";
	//string url = "F:\\car_rect_plate_test.mp4";
	//string url = "F:\\FIT8\\short4.ts";
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