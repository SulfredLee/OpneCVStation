// OpenCVStation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
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
#include "BasicMotionDetection.h"
#include "BookTracking.h"
#include "WhereWaldo.h"
#include "BackGround2White.h"
#include "SplitMovie.h"
#include "VideoReader.h"
#include "ExtractSubtitle.h"
#include "Scanner.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//BasicMotionDetection(argc, argv);
	//BookTracking(argc, argv);
	//WhereWaldo(argc, argv);
	//BackGround2White(argc, argv);
	//RemoveSubTitle app;
	//ExtractSubtitle app;
	Scanner app;
	app.main(argc, argv);
	return 0;
}

