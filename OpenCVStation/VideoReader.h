#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

#include <opencv2/core/core.hpp>
#include <opencv2/core/opengl_interop.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>

class VideoReader
{
public:
	VideoReader(){}
	~VideoReader(){}

	int main(int argc, _TCHAR* argv[]);
protected:
};
#endif