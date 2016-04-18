#include "KNNTextRecognizer.h"
#include <ctime>

void convertToGray(cv::Mat& img);
void convertToStandardSize(cv::Mat& img, int maxSize);
void wrapImg(cv::Mat& img, cv::Size size);
std::vector<cv::Rect> findCharacterRect(cv::Mat& img, cv::Mat& proccessedImg);

void pr::KNNTextRecognizer::Init(std::string indexFile, std::string dataFile)
{
	cv::FileStorage fsClassification(indexFile, cv::FileStorage::READ);
	fsClassification["classifications"] >> matClassifyIndex;
	fsClassification.release();
		
	cv::FileStorage fsTrainingImages(dataFile, cv::FileStorage::READ);
	fsTrainingImages["images"] >> matClassifyData;
	fsTrainingImages.release();

	knearst = cv::Ptr<cv::ml::KNearest>(cv::ml::KNearest::create());
	knearst->train(matClassifyData, cv::ml::ROW_SAMPLE, matClassifyIndex);

}

char pr::KNNTextRecognizer::GetCharacter(cv::Mat& img)
{
	ProcessImg(img);
	cv::Mat imgFloat;
	img.convertTo(imgFloat, CV_32FC1);
	cv::Mat imgFeatureFloat = imgFloat.reshape(1, 1);

	cv::Mat matCurrentChar(0, 0, CV_32F);
	knearst->findNearest(imgFeatureFloat, neighbor, matCurrentChar);

	float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);

	imgFloat.release();
	imgFeatureFloat.release();
	matCurrentChar.release();

	return char(int(fltCurrentChar));
}

void pr::KNNTextRecognizer::ProcessImg(cv::Mat& img)
{
	convertToGray(img);
	convertToStandardSize(img, imgMaxEdgeSize);
	wrapImg(img, imgWrapSize);
}

void pr::KNNTextRecognizer::SetImgMaxEdgeSize(int size)
{
	this->imgMaxEdgeSize = size;
}

void pr::KNNTextRecognizer::SetImgWrapSize(cv::Size size)
{
	this->imgWrapSize = size;
}

bool isFront(cv::Rect r1, cv::Rect r2){
	if (r1.y - 30 < r2.y && r1.y + 30 > r2.y){
		return r1.x > r2.x;
	}
	else{
		return true;
	}
		
}

void sort(std::vector<cv::Rect>& rect){
	std::cout << "Rect size: " << rect.size() << std::endl;
	if (!rect.empty() && rect.size() > 0){
		cv::Rect tmp;	
		for (int i = 0; i < rect.size()-1; i++){
			for (int j = i + 1; j < rect.size(); j++){
				if (isFront(rect[i], rect[j])){
					tmp = rect[i];
					rect[i] = rect[j];
					rect[j] = tmp;
				}
			}
		}
	}
}

std::string pr::KNNTextRecognizer::GetText(cv::Mat& img)
{	
	//return std::string(1, GetCharacter(img));
	int start_s = clock();

	std::string result="";	
	cv::Mat processedImg;
	if (!img.empty()){
		std::vector<cv::Rect> rect = findCharacterRect(img, processedImg);	
		sort(rect);
		for (int i = 0; i < rect.size(); i++){
			//imshow(std::to_string(i), processedImg(rect[i]));
			result += GetCharacter(processedImg(rect[i]));
		}

		int stop_s = clock();
	std::cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << std::endl;
	}
	imshow("processed Img", processedImg);
	imshow("original plate", img);
	return result;
}

void convertToGray(cv::Mat& img){
	if (img.channels() != 1){
		cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);
		//cv::equalizeHist(img, img);
	}
}

void convertToStandardSize(cv::Mat& img, int maxSize){
	cv::Size imgSize = img.size();
	float ratio = (float)imgSize.width / imgSize.height;
	//cout << ratio << endl;
	cv::Size scaleSize;
	if (ratio > 1){
		scaleSize.width = maxSize;
		scaleSize.height = maxSize / ratio;
	}
	else if (ratio < 1){
		scaleSize.height = maxSize;
		scaleSize.width = maxSize * ratio;
	}
	else{
		scaleSize = cv::Size(maxSize, maxSize);
	}
	cv::resize(img, img, scaleSize);
}

void wrapImg(cv::Mat& img, cv::Size size){
	cv::Mat result(size.width, size.height, CV_8UC1, cv::Scalar(255, 255, 255));
	int x = (size.width - img.size().width) / 2;
	int y = (size.height - img.size().height) / 2;
	//std::cout << x << " " << y << std::endl;
	img.copyTo(result(cv::Rect(x, y, img.cols, img.rows)));
	img = result;
	//imshow("result", result);
}

// --- TESTING ------------------------------------
int _scaleFactor = 3;
int _minCharWidth = 2 * _scaleFactor;
int _minCharHeight = 10 * _scaleFactor;
int _maxCharWidth = 40 * _scaleFactor;
int _maxCharHeight = 40 * _scaleFactor;

bool isValidCharBox(cv::Rect rect){
	return rect.width > _minCharWidth && rect.height > _minCharHeight && rect.width < _maxCharWidth && rect.height < _maxCharHeight;
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
	proccessedImg = customeImgProcess.clone();

	int erosion_size = 1;
	cv::Mat element = cv::getStructuringElement(
		cv::MORPH_RECT,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));
	cv::dilate(customeImgProcess, customeImgProcess, element);

	cv::findContours(customeImgProcess, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	std::vector<std::vector<cv::Point> >contours_polys = std::vector<std::vector<cv::Point> >(contours.size());
	std::vector<cv::Rect>boundRects = std::vector<cv::Rect>(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contours[i]), contours_polys[i], 1, true);
		boundRects[i] = cv::boundingRect(cv::Mat(contours_polys[i]));
		if (isValidCharBox(boundRects[i])){
			cv::Scalar color = cv::Scalar(255, 0, 255);
			rectangle(img, boundRects[i].tl(), boundRects[i].br(), color, 2, 8, 0);
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