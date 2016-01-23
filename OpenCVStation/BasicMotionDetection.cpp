#include "stdafx.h"
#include "BasicMotionDetection.h"

void BasicMotionDetection(int argc, _TCHAR* argv[]){
	if(argc < 7){
		std::cerr << "Usage : BasicMotionDetection.exe -s sourceFile -min minArea -o outputFile\n";
		exit(0);
	}

	ParsingCMD parserCMD;
	parserCMD.Set("-s"); // --source
	parserCMD.Set("-min"); // --minimum area
	parserCMD.Set("-o"); // --out
	
	parserCMD.DoParsing(argc, argv);

	cv::VideoCapture cap(parserCMD["-s"]);

	cv::Mat frame, firstFrame;
	bool firstLoop = true;
	while(cap.read(frame)){
		ResizeHeight(frame, frame, 500);
		ImageCrop(frame, frame, 10, 5, 600, 450);
		cv::Mat gray;
		cv::cvtColor(frame, gray, CV_RGB2GRAY);
		cv::GaussianBlur(gray, gray, cv::Size(21, 21), 0, 0);

		if(firstLoop){
			gray.copyTo(firstFrame);
			firstLoop = false;
			continue;
		}

		cv::Mat frameDelta;
		cv::absdiff(firstFrame, gray, frameDelta);
		cv::Mat thresh;
		cv::threshold(frameDelta, thresh, 20, 255, CV_THRESH_BINARY);
		
		cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 5);
		
		std::vector<std::vector<cv::Point> > cnts;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(thresh, cnts, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		for(size_t i = 0; i < cnts.size(); i++){
			if(cv::contourArea(cnts[i]) <= std::stod(parserCMD["-min"])){
				continue;
			}

			cv::Rect objectBound = cv::boundingRect(cnts[i]);
			cv::rectangle(frame, objectBound, cv::Scalar(0, 255, 0), 2);
		}

		cv::putText(frame, "Room Status: {}", cv::Point(10, 20), CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, (0, 0, 255), 1);

		cv::imshow("Security Feed", frame);
		//cv::imshow("Thresh", thresh);
		//cv::imshow("Frame Delta", frameDelta);

		if(cv::waitKey(1) & 0xFF == 'q'){
			break;
		}
	}
}