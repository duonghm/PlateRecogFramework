#include <iostream>
#include <CameraConvertor.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//#define TEST_CAMERA_CONVERTOR
//#define TEST_PLATE_DETECTOR
//#define TEST_PLATE_EXTRACTOR
//#define TEST_PLATE_RECOGNIZATOR
#define TEST_VLC

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
#define VIDEO_WIDTH     2592/2
#define VIDEO_HEIGHT    1920/2

PlateRecognizator recognizator_rect;
std::vector<pr::PlateRegion> plates_rect;

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
		cv::Mat dis = frame.clone();
		recognizator_rect.SetImg(frame);		
		plates_rect = recognizator_rect.GetPlateRegions();
		//std::cout << plates_rect.size() << endl;
		for (int i = 0; i < plates_rect.size(); i++){
			cv::rectangle(dis, plates_rect[i].region, cv::Scalar(255, 0, 255), 2, 8, 0);
		}
		
		cv::imshow("frame", dis);

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
	recognizator_rect.Init("../data/plateCascade/vn_2.xml");

	string url = "F:\\FIT8\\fit8.ts";
	//vlc sdk does not know the video size until it is rendered, so need to play it a bit so that size is     known
	cv::Size sz = cv::Size(VIDEO_WIDTH, VIDEO_HEIGHT);

	// VLC pointers
	libvlc_instance_t *vlcInstance;
	libvlc_media_player_t *mp;
	libvlc_media_t *media;

	char *vlc_argv[6];
	vlc_argv[0] = new char[3];
	strcpy_s(vlc_argv[0], 3, "-I");
	vlc_argv[1] = new char[6];
	strcpy_s(vlc_argv[1], 6, "dummy"); // Don't use any interface
	vlc_argv[2] = new char[16];
	strcpy_s(vlc_argv[2], 16, "--ignore-config"); // Don't use VLC's config
	//vlc_argv[3] = new char[128];
	//strcpy_s(vlc_argv[3], 128, "--plugin-path=/plugins");
	int vlc_argc = 3;

	vlcInstance = libvlc_new(vlc_argc, vlc_argv);

	for (int i = 0; i < vlc_argc; i++)
		delete[] vlc_argv[i];

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

	return 0;
}