#ifndef EXTRACTSUBTITLE_H
#define EXTRACTSUBTITLE_H

#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\gpu\gpu.hpp>
#include <string>
#include "sfOpenCVLib.h"
#include <fstream>
#include <ctime>

class ExtractSubtitle
{
public:
	int main(int argc, _TCHAR* argv[]);
private:
	std::string fileName;
	std::string outputDirectory;
	cv::VideoCapture cap;
	MovieStat movieStat;
	float x, y, w, h;
	std::ofstream outFH;
};

#endif