#include "stdafx.h"
#include "ExtractSubtitle.h"

int ExtractSubtitle::main(int argc, _TCHAR* argv[])
{
	if (argc < 13){
		std::cerr << "Usage:\n"
			<< "ExtractSubtitle.exe -f fileName -d outputDirectory -x x-axis -y y-axis -w width -h height\n";
		return -1;
	}

	ParsingCMD cmd;
	cmd.Set("-f"); cmd.Set("-d"); cmd.Set("-x"); cmd.Set("-y"); cmd.Set("-w"); cmd.Set("-h");
	cmd.DoParsing(argc, argv);
	fileName = cmd["-f"];
	outputDirectory = cmd["-d"];
	x = std::stof(cmd["-x"]); y = std::stof(cmd["-y"]);
	w = std::stof(cmd["-w"]); h = std::stof(cmd["-h"]);
	cap.open(fileName);

	GetMovieStat(movieStat, cap);

	cv::Mat curFrame, preFrameGray, curFrameGray;
	int curImg = 1;
	int curFrameCount = 0;
	cv::Rect myROI(x, y, w, h);
	std::clock_t c_start = std::clock();
	std::clock_t first_start = std::clock();
	outFH.open(outputDirectory + "\\timeInfo.txt", std::ios::out);
	cap.read(curFrame);
	cv::cvtColor(curFrame(myROI), preFrameGray, CV_RGB2GRAY);
	while (cap.read(curFrame)){
		if (curFrameCount++ % 3 == 0)
			continue;
		cv::Mat croppedImage = curFrame(myROI);
		cv::cvtColor(croppedImage, curFrameGray, CV_RGB2GRAY);
		//cv::Mat diffFrame;
		//cv::absdiff(preFrameGray, curFrameGray, diffFrame);
		char outputFile[512];
		sprintf_s(outputFile, "%s\\%06d.png", outputDirectory.c_str(), ++curImg);
		cv::Mat inverse = cv::Scalar::all(255) - curFrameGray;
		cv::adaptiveThreshold(inverse, inverse, 250, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 11, 20);
		cv::imwrite(outputFile, inverse);

		outFH << outputFile << " " << cap.get(CV_CAP_PROP_POS_MSEC) << std::endl;

		if (curFrameCount % (static_cast<int>(movieStat.frameRate) * 60) == 0){
			std::cout << "Process one Minute of Movie used "
				<< float(clock() - c_start) / CLOCKS_PER_SEC << " sec.\n"
				<< "Total sec spent " << float(clock() - first_start) / CLOCKS_PER_SEC << "\n"
				<< "Processed " << curFrameCount / movieStat.totalFrames << "\n";
			c_start = std::clock();
		}
		curFrameGray.copyTo(preFrameGray);
	}
	outFH.close();
}