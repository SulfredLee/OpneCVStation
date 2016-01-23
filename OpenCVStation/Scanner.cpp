#include "stdafx.h"
#include "Scanner.h"

cv::Mat Scanner::four_point_transform(const cv::Mat& image, const std::vector<cv::Point>& pts, const double& ratio){
	cv::Mat outMat;
	cv::Point TL, TR, BR, BL;

	order_points(pts, TL, TR, BR, BL);
	TL *= ratio;
	TR *= ratio;
	BR *= ratio;
	BL *= ratio;

	double widthA = std::sqrt((BR.x - BL.x)*(BR.x - BL.x) + (BR.y - BL.y)*(BR.y - BL.y));
	double widthB = std::sqrt((TR.x - TL.x)*(TR.x - TL.x) + (TR.y - TL.y)*(TR.y - TL.y));
	int maxWidth = std::max(static_cast<int>(widthA), static_cast<int>(widthB));

	double heightA = std::sqrt((TR.x - BR.x)*(TR.x - BR.x) + (TR.y - BR.y)*(TR.y - BR.y));
	double heightB = std::sqrt((TL.x - BL.x)*(TL.x - BL.x) + (TL.y - BL.y)*(TL.y - BL.y));
	int maxHeight = std::max(static_cast<int>(heightA), static_cast<int>(heightB));

	cv::Point2f orgAry[4];
	orgAry[0] = TL;
	orgAry[1] = TR;
	orgAry[2] = BR;
	orgAry[3] = BL;

	cv::Point2f dstAry[4];
	dstAry[0] = cv::Point(0, 0);
	dstAry[1] = cv::Point(maxWidth - 1, 0);
	dstAry[2] = cv::Point(maxWidth - 1, maxHeight - 1);
	dstAry[3] = cv::Point(0, maxHeight - 1);

	cv::Mat M = cv::getPerspectiveTransform(orgAry, dstAry); // These two inputs should have the same order
	cv::warpPerspective(image, outMat, M, cv::Size(maxWidth, maxHeight));
	return outMat;
}
bool Scanner::SortVecPoint(const std::vector<cv::Point>& first, const std::vector<cv::Point>& sencond){
	return (cv::contourArea(first) > cv::contourArea(sencond)); // from large area to small area
}
void Scanner::GetRatio(){
	newHeight = 500.0;
	ratio = image.size().height / newHeight;
	invRatio = 1 / ratio;
}
void Scanner::DrawCont(const std::vector<std::vector<cv::Point> >& inCont){
	cv::RNG rng(12345);
	cv::Mat drawing = cv::Mat::zeros(edged.size(), CV_8UC3);
	for (int i = 0; i< inCont.size(); i++)
	{
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, inCont, i, color, 2, 8, hierarchy, 0, cv::Point());
		imshow("Contours", drawing);
		if (cv::waitKey(30) >= 0) break;
	}
}
int Scanner::main(int argc, _TCHAR* argv[])
{
	if (argc < 5){
		std::cerr << "Usage:\n"
			<< "Scanner.exe -i imgFile -s shiftValue\n";
		return -1;
	}

	ParsingCMD cmd;
	cmd.Set("-i"); cmd.Set("-s");
	cmd.DoParsing(argc, argv);

	fileName = cmd["-i"];
	shiftValue = std::atoi(cmd["-s"].c_str());

	image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
	GetRatio();
	image.copyTo(orig);
	cv::resize(image, image, cv::Size(), invRatio, invRatio);


	cv::cvtColor(image, gray, CV_BGR2GRAY);
	cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
	int temptemp = 20;
	cv::Canny(gray, edged, temptemp, temptemp * 2);


	cv::findContours(edged, cnts, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	std::sort(cnts.begin(), cnts.end(), Scanner::SortVecPoint);
	//DrawCont(cnts);

	for (size_t i = 0; i < cnts.size(); i++){
		double peri = cv::arcLength(cnts[i], true);
		std::vector<cv::Point> approx;
		cv::approxPolyDP(cv::Mat(cnts[i]), approx, 0.02 * peri, true);

		if (approx.size() == 4){
			screenCnt.push_back(approx);
			break;
		}
	}

	warped = four_point_transform(orig, screenCnt[0], ratio);

	cv::cvtColor(warped, warped, CV_BGR2GRAY);
	cv::adaptiveThreshold(warped, warped, 250, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 11, shiftValue);
	cv::imwrite("out" + fileName, warped);

	return 0;
}