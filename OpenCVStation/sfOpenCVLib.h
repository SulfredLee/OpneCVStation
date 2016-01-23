#ifndef SFOPENCVLIB_H
#define SFOPENCVLIB_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <map>

std::string type2str(int type);

std::string Argv2String(const _TCHAR* temp);
void ResizeHeight(const cv::Mat& in, cv::Mat& out, const int& height);
void ResizeWidth(const cv::Mat& in, cv::Mat& out, const int& width);
void ImageCrop(const cv::Mat& in, cv::Mat& out, const int& x, const int& y, int width, int height);
void ImageInsert(const cv::Mat& in, cv::Mat& out, const int& x, const int& y, int width, int height);
bool DrawCont(const std::vector<std::vector<cv::Point> >& inCont, const cv::Mat& image);


void FindTopLeft(std::vector<cv::Point>& pts);
void FindBottomRight(std::vector<cv::Point>& pts);
void FindTopRight(std::vector<cv::Point>& pts);
void order_points(std::vector<cv::Point> pts, cv::Point& TL, cv::Point& TR, cv::Point& BR, cv::Point& BL);

// string operation
std::vector<std::string> string_split(const std::string& s, const char& delim);

struct MovieStat{
	double width;
	double height;
	double totalFrames;
	double frameRate;
};
void GetMovieStat(MovieStat& movieStat, cv::VideoCapture& cap);

class ParsingCMD{
private:
	std::map<std::string, bool> argvTable;
	std::map<std::string, std::string> insideArgv;
public:
	ParsingCMD(){}
	~ParsingCMD(){}

	void Set(const std::string& inArgv){
		argvTable[inArgv] = true;
	}
	void DoParsing(int argc, _TCHAR* argv[]){
		for(int i = 1; i < argc; ){
			if(argvTable.find(Argv2String(argv[i])) != argvTable.end()){
				insideArgv[Argv2String(argv[i])] = Argv2String(argv[i+1]);
				i += 2;
			}else{
				i++;
			}
		}
	}
	std::string operator [](const std::string& in){
		return insideArgv[in];
	}
};

#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif


#endif