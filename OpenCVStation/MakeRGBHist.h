#include "sfOpenCVLib.h"

void GetDataList();
std::vector<float> ConvertMat2VectorFloat(const cv::Mat& m);
std::vector<float> FeatureExtrac(const std::string& inImageFile, const std::vector<int>& sizeVector);
void SaveFeature(const std::string& outFile, const std::vector<float>& outData);
void MakeRGBHist(int argc, _TCHAR* argv[]);