// MakeRGBHist.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MakeRGBHist.h"

std::vector<std::string> inDataList;
std::vector<std::string> outDataList;
size_t dataNum;
std::string dataFile;
std::vector<int> sizeVector(3); // int binSizeR, binSizeG, binSizeB;




void GetDataList(){
	std::ifstream FH(dataFile);

	dataNum = 0;
	std::string line;
	while (std::getline(FH, line)){
		std::vector<std::string> parts = string_split(line, ' ');
		inDataList.push_back(parts[0]);
		outDataList.push_back(parts[1]);
		dataNum++;
	}
	FH.close();
}
std::vector<float> ConvertMat2VectorFloat(const cv::Mat& m){
	//std::cout << type2str(hist.type());
	if (type2str(m.type()) != "32FC1"){
		std::vector<float> outVec;
		return outVec;
	}
	if (m.isContinuous()){
		const float* p = m.ptr<float>(0);
		std::vector<float> outVec(p, p + m.total());
		return outVec;
	}
	std::vector<float> outVec;
	return outVec;
}
std::vector<float> FeatureExtrac(const std::string& inImageFile, const std::vector<int>& sizeVector){
	cv::Mat hist;


	cv::Mat image = cv::imread(inImageFile);
	int histSize[3] = { sizeVector[0], sizeVector[1], sizeVector[2] };
	float range[2] = { 0, 256 };
	const float * ranges[3] = { range, range, range };
	int channels[3] = { 0, 1, 2 };
	calcHist(&image, 1, channels, cv::Mat(), hist, 3, histSize, ranges);
	cv::normalize(hist, hist);

	return ConvertMat2VectorFloat(hist);
}
void SaveFeature(const std::string& outFile, const std::vector<float>& outData){
	if (outData.size() == 0)
		return;

	std::ofstream FH(outFile, std::ios::out | std::ios::binary);
	FH.write(reinterpret_cast<const char*>(&outData[0]), outData.size()*sizeof(float));
	FH.close();
}
void MakeRGBHist(int argc, _TCHAR* argv[])
{
	if (argc < 5){
		std::cerr << "Usage:\n"
			<< "MakeRGBHist.exe DataList BinSizeR BinSizeG BinSizeB\n";
		exit(0);
	}

	dataFile = Argv2String(argv[1]);
	sizeVector[0] = (int)std::stod(Argv2String(argv[2]).c_str()); // binSizeR
	sizeVector[1] = (int)std::stod(Argv2String(argv[3]).c_str()); // binSizeG
	sizeVector[2] = (int)std::stod(Argv2String(argv[4]).c_str()); // binSizeB

	GetDataList();

	for (size_t i = 0; i < dataNum; i++){
		SaveFeature(outDataList[i], FeatureExtrac(inDataList[i], sizeVector));
	}
}

