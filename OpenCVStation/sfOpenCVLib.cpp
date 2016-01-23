#include "stdafx.h"
#include "sfOpenCVLib.h"


std::string Argv2String(const _TCHAR* temp){
	String Arg = temp;
	std::string out(Arg.begin(), Arg.end());
	return out;
}
void ResizeHeight(const cv::Mat& in, cv::Mat& out, const int& height){
	double ratio = static_cast<double>(height) / in.size().height;
	cv::Mat tempOut;
	cv::resize(in, tempOut, cv::Size(), ratio, ratio);
	tempOut.copyTo(out);
}
void ResizeWidth(const cv::Mat& in, cv::Mat& out, const int& width){
	double ratio = static_cast<double>(width) / in.size().width;
	cv::Mat tempOut;
	cv::resize(in, tempOut, cv::Size(), ratio, ratio);
	tempOut.copyTo(out);
}
bool DrawCont(const std::vector<std::vector<cv::Point> >& inCont, const cv::Mat& image){
	if(inCont.size() == 0)
		return false;

	cv::RNG rng(12345);
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat drawing = cv::Mat::zeros(image.size(), CV_8UC3);
	for (int i = 0; i< inCont.size(); i++)
	{
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, inCont, i, color, 2, 8, hierarchy, 0, cv::Point());
		imshow("Contours", drawing);
		if (cv::waitKey(30) >= 0) break;
	}

	return true;
}
void ImageCrop(const cv::Mat& in, cv::Mat& out, const int& x, const int& y, int width, int height){
	int originWidth = in.size().width;
	int originHeight = in.size().height;
	
	if(x+width > originWidth)
		width = originWidth - x;
	if(y+height > originHeight)
		height = originHeight - y;

	cv::Rect myROI(x, y, width, height);
	cv::Mat tempout = in(myROI);
	tempout.copyTo(out);
}
void ImageInsert(const cv::Mat& in, cv::Mat& out, const int& x, const int& y, int width, int height){
	cv::Rect roi(x, y, width, height);
	cv::Mat destinationROI = out(roi);
	in.copyTo(destinationROI);
}
void FindTopLeft(std::vector<cv::Point>& pts){
	double sum = pts[0].x + pts[0].y;
	size_t minIndex = 0;
	for (size_t i = 1; i < pts.size(); i++){
		if (sum > pts[i].x + pts[i].y){
			sum = pts[i].x + pts[i].y;
			minIndex = i;
		}
	}
	cv::Point temp = pts[minIndex];
	pts[minIndex] = pts[0];
	pts[0] = temp;
}
void FindBottomRight(std::vector<cv::Point>& pts){
	double sum = pts[0].x + pts[0].y;
	size_t maxIndex = 0;
	for (size_t i = 1; i < pts.size(); i++){
		if (sum < pts[i].x + pts[i].y){
			sum = pts[i].x + pts[i].y;
			maxIndex = i;
		}
	}
	cv::Point temp = pts[maxIndex];
	pts[maxIndex] = pts[2];
	pts[2] = temp;
}
void FindTopRight(std::vector<cv::Point>& pts){
	if (pts[1].y > pts[3].y){
		cv::Point temp = pts[1];
		pts[1] = pts[3];
		pts[3] = temp;
	}
}
void order_points(std::vector<cv::Point> pts, cv::Point& TL, cv::Point& TR, cv::Point& BR, cv::Point& BL){
	// The order of these functions cannot be changed
	// Four points order is
	// pts[0] = TopLeft, pts[1] = TopRight, pts[2] = BottomRight, pts[3] = BottomLeft
	FindTopLeft(pts);
	FindBottomRight(pts);
	FindTopRight(pts);

	TL = pts[0];
	TR = pts[1];
	BR = pts[2];
	BL = pts[3];
}
std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
std::vector<std::string> string_split(const std::string& s, const char& delim){
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;

}
void GetMovieStat(MovieStat& movieStat, cv::VideoCapture& cap){
	movieStat.width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	movieStat.height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	movieStat.totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT);
	movieStat.frameRate = cap.get(CV_CAP_PROP_FPS);

	std::cout << "Movie width: " << movieStat.width << "\n";
	std::cout << "Movie height: " << movieStat.height << "\n";
	std::cout << "Movie totalFrames: " << movieStat.totalFrames << "\n";
	std::cout << "Movie frameRate: " << movieStat.frameRate << "\n";
}