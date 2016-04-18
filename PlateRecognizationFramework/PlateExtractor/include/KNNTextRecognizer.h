#ifndef KNN_TEXT_RECOGNIZER_H
#define KNN_TEXT_RECOGNIZER_H
#include "opencv2/opencv.hpp"
#include "opencv2/ml.hpp"
#include "ITextRecognizer.h"

namespace pr{
	class KNNTextRecognizer : ITextRecognizer{
	private:
		cv::Mat matClassifyIndex;
		cv::Mat matClassifyData;
		cv::Ptr<cv::ml::KNearest> knearst;

		int imgMaxEdgeSize = 25;
		cv::Size imgWrapSize = cv::Size(28, 28);
		int neighbor = 1;
	public:	
		void ProcessImg(cv::Mat& img);
		void Init(std::string indexFile, std::string dataFile);
		char GetCharacter(cv::Mat& img);
		void SetImgMaxEdgeSize(int size);
		void SetImgWrapSize(cv::Size size);

		std::string GetText(cv::Mat& img) override;

	};
}

#endif // !KNN_TEXT_RECOGNIZER_H
