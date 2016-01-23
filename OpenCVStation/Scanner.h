#ifndef SCANNER_H
#define SCANNER_H


#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <fstream>
#include "sfOpenCVLib.h"


class Scanner
{
public:
	Scanner(){}
	~Scanner(){}

	int main(int argc, _TCHAR* argv[]);
private:
	std::string fileName;
	cv::Mat image, orig, gray, edged, warped, warped2;
	std::vector<std::vector<cv::Point> > cnts;
	double ratio;
	double invRatio;
	double newHeight;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > screenCnt;	
	int shiftValue;

	void GetRatio();
	static bool SortVecPoint(const std::vector<cv::Point>& first, const std::vector<cv::Point>& sencond);
	cv::Mat four_point_transform(const cv::Mat& image, const std::vector<cv::Point>& pts, const double& ratio);
	void DrawCont(const std::vector<std::vector<cv::Point> >& inCont);
};

#endif