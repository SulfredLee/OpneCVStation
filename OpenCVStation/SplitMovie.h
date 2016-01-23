#ifndef SPLITMOVIE_H
#define SPLITMOVIE_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <ctime>
#include <iomanip>
#include <fstream>

#include <opencv2\opencv.hpp>
#include <opencv2\gpu\gpu.hpp>

#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif

class SplitMovie
{
public:
	struct MovieStat{
		double width;
		double height;
		double totalFrames;
		double frameRate;
	};
	struct Period{
		std::string start;
		std::string end;
	};


	SplitMovie(){}
	~SplitMovie(){}

	int Split(int argc, _TCHAR* argv[]);
protected:
	std::vector<int> timeList; // stored blocks location
	std::vector<int> timeListAll; // stored frame time along the whole movie
	std::vector<double> meanList; // stored mean power per frame along the whole movie
	cv::VideoCapture cap;
	MovieStat movieStat;
	std::string fileName;
	std::string choice;
	double threshold;
	int lastFrameTime;
	std::vector<Period> periodList;
	std::vector<double> minPowers;
	std::vector<double> maxPowers;

	std::string double2string(const double& in);
	std::string sfGetSubStr(const std::string& target, const std::string& pattern);
	std::string int2string(const int& in);
	double MatMean(const cv::Mat& frame);
	std::string Argv2String(const _TCHAR* temp);
	double Argv2Double(const _TCHAR* temp);
	void GetMovieStat();
	void FileChecking();
	bool isBlock(const double& value);
	std::vector<double> MakeSteps(const double& value, const double& step, const int& iter);
	void OutPutGNUCmd(const std::string& fileName, const std::string& dataFile);
	void OutPutPowerStat(const std::string& fileName);
	void GetBlockTime();
	void FillUpTimeList();
	void UniqTimeList();
	std::vector<int> CleanList(const std::vector<int>& list);
	std::vector<int> MakeEndList(const std::vector<int>& list);
	std::vector<int> MakeStartList(std::vector<int> list);
	int Sec2Hour(const int& sec);
	int Sec2Min(const int& sec);
	int Sec2Sec(const int& sec);
	std::string Sec2TimeString(const int& sec);
	std::vector<Period> MakePeriodList(const std::vector<int>& startList, const std::vector<int>& endList);
	void MakePeriodList();
	void CallFunction();
	void ProcessWholeMovie();
	void OutPutText();
	void ProcessStack();
	void ProcessMinCases();
	void ProcessMaxCases();
};
#endif