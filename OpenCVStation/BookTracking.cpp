#include "stdafx.h"
#include "BookTracking.h"

cv::Mat frame;
std::vector<cv::Point> rioPts;
bool inputMode = false;

static void selectROI(int event, int x, int y, int flags, void* param){
	if(inputMode && event == CV_EVENT_LBUTTONDOWN && rioPts.size() < 4){
		rioPts.push_back(cv::Point(x, y));
		cv::circle(frame, cv::Point(x, y), 4, cv::Scalar(0, 255, 0), 2);
		cv::imshow("frame", frame);
	}
}
void BookTracking(int argc, _TCHAR* argv[]){
	if(argc < 3){
		std::cerr << "Usage : BookTracking.exe -s sourceFile\n";
		exit(0);
	}

	ParsingCMD parserCMD;
	parserCMD.Set("-s"); // --source	
	parserCMD.DoParsing(argc, argv);
	
	cv::VideoCapture cap(parserCMD["-s"]);

	cv::namedWindow("frame");
	cv::setMouseCallback("frame", selectROI, 0);

	cv::TermCriteria termination = cv::TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 );
	cv::Rect roiBox;
	cv::MatND roiHist;
	while(cap.read(frame)){		
		if(roiBox.area() != 0){
			cv::Mat hsv;
			cv::MatND backProj;
			cv::cvtColor(frame, hsv, CV_BGR2HSV);

			float hue_range[2] = { 0, 180 };
			const float* ranges[1] = { hue_range };
			int channels[1] = { 0 };
			cv::calcBackProject(&hsv, 1, 0, roiHist, backProj, ranges, 1);

			cv::Point2f rectPoints[4];
			cv::RotatedRect roiBoxRotated = cv::CamShift(backProj, roiBox, termination);
			roiBoxRotated.points(rectPoints);
			int npts = 4;
			//cv::polylines(frame, rectPoints, &npts, 1, true, cv::Scalar(0, 255, 0), 2);
			cv::ellipse(frame, roiBoxRotated, cv::Scalar(0, 0, 255), 2);
		}
		cv::imshow("frame", frame);
		char key = cv::waitKey(1) & 0xFF;

		if(key == 'i' && rioPts.size() < 4){
			inputMode = true;
			cv::Mat orig;
			frame.copyTo(orig);

			while(rioPts.size() < 4){
				cv::imshow("frame", frame);
				cv::waitKey(0);
			}

			cv::Point TL, TR, BR, BL;
			order_points(rioPts, TL, TR, BR, BL);

			cv::Mat roi;
			ImageCrop(orig, roi, TL.x, TL.y, BR.x-TL.x, BR.y-TL.y);
			cv::cvtColor(roi, roi, CV_BGR2HSV);

			float range[2] = { 0, 180 } ;   const float* histRange[1] = { range };
			int histSize[1] = { 16 };
			int channels[1] = {0};
			cv::calcHist(&roi, 1, channels, cv::Mat(), roiHist, 1, histSize, histRange);
			cv::normalize(roiHist, roiHist, 0, 255, CV_MINMAX);

			roiBox.x = TL.x;
			roiBox.y = TL.y;
			roiBox.width = BR.x-TL.x;
			roiBox.height = BR.y-TL.y;
		}else if(key == 'q')
			break;
	}
	cv::destroyAllWindows();
}