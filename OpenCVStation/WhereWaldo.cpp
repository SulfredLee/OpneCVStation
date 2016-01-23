#include "stdafx.h"
#include "WhereWaldo.h"

void WhereWaldo(int argc, _TCHAR* argv[])
{
	if (argc < 5){
		std::cerr << "Usage : WhereWaldo.exe -p puzzle -w waldo\n";
		exit(0);
	}

	ParsingCMD parserCMD;
	parserCMD.Set("-p"); // --puzzle
	parserCMD.Set("-w"); // --waldo
	parserCMD.DoParsing(argc, argv);

	cv::Mat puzzle = cv::imread(parserCMD["-p"]);
	cv::Mat waldo = cv::imread(parserCMD["-w"]);
	double waldoHeight = waldo.size().height;
	double waldoWidth = waldo.size().width;

	cv::Mat result;
	cv::matchTemplate(puzzle, waldo, result, CV_TM_CCOEFF);

	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

	cv::Point topLeft = maxLoc;
	cv::Point botRight;
	botRight.x = topLeft.x + waldoWidth; botRight.y = topLeft.y + waldoHeight;
	cv::Mat roi;
	ImageCrop(puzzle, roi, topLeft.x, topLeft.y, waldoWidth, waldoHeight);

	cv::Mat mask = cv::Mat::zeros(puzzle.size().height, puzzle.size().width, CV_8UC3);
	std::cout << puzzle.channels() << " " << mask.channels();
	cv::addWeighted(puzzle, 0.25, mask, 0.75, 0, puzzle);

	ImageInsert(roi, puzzle, topLeft.x, topLeft.y, waldoWidth, waldoHeight);

	ResizeHeight(puzzle, puzzle, 500);
	ResizeHeight(result, result, 500);
	cv::imshow("Puzzle", puzzle);
	cv::imshow("Waldo", waldo);
	cv::waitKey(0);
}