#include "stdafx.h"
#include "BackGround2White.h"

void BackGround2White(int argc, _TCHAR* argv[]){
	if (argc < 7){
		std::cerr << "Usage : BackGround2White.exe -s source -o output -sh shiftValue\n";
		exit(0);
	}

	ParsingCMD parserCMD;
	parserCMD.Set("-s"); // --source
	parserCMD.Set("-o"); // --output
	parserCMD.Set("-sh");
	parserCMD.DoParsing(argc, argv);

	cv::Mat img = cv::imread(parserCMD["-s"]);
	cv::Mat gray;
	if (img.channels() == 3)
		cv::cvtColor(img, gray, CV_BGR2GRAY);
	else
		img.copyTo(gray);

	int shiftValue = std::atoi(parserCMD["-sh"].c_str());
	cv::adaptiveThreshold(gray, gray, 250, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 11, shiftValue);
	cv::imwrite(parserCMD["-o"], gray);

	return;
}