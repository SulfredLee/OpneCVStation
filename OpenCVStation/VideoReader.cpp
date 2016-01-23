#include "stdafx.h"
#include "VideoReader.h"
#include "sfOpenCVLib.h"

int VideoReader::main(int argc, _TCHAR* argv[]){
	if (argc < 3){
		std::cerr << "Usage:\n"
			<< "VideoReader.exe -f fileName\n";
		return -1;
	}
		

	ParsingCMD cmd;
	cmd.Set("-f");
	cmd.DoParsing(argc, argv);

	const std::string fname(cmd["-f"]);

	cv::namedWindow("CPU", cv::WINDOW_NORMAL);
	cv::namedWindow("GPU", cv::WINDOW_OPENGL);
	cv::gpu::setGlDevice();

	cv::Mat frame;
	cv::VideoCapture reader(fname);

	cv::gpu::GpuMat d_frame;
	cv::gpu::VideoReader_GPU d_reader(fname);
	d_reader.dumpFormat(std::cout);

	cv::TickMeter tm;
	std::vector<double> cpu_times;
	std::vector<double> gpu_times;

	for (;;)
	{
		tm.reset(); tm.start();
		if (!reader.read(frame))
			break;
		tm.stop();
		cpu_times.push_back(tm.getTimeMilli());

		tm.reset(); tm.start();
		if (!d_reader.read(d_frame))
			break;
		tm.stop();
		gpu_times.push_back(tm.getTimeMilli());

		cv::imshow("CPU", frame);
		cv::imshow("GPU", d_frame);

		if (cv::waitKey(3) > 0)
			break;
	}

	if (!cpu_times.empty() && !gpu_times.empty())
	{
		std::cout << std::endl << "Results:" << std::endl;

		std::sort(cpu_times.begin(), cpu_times.end());
		std::sort(gpu_times.begin(), gpu_times.end());

		double cpu_avg = std::accumulate(cpu_times.begin(), cpu_times.end(), 0.0) / cpu_times.size();
		double gpu_avg = std::accumulate(gpu_times.begin(), gpu_times.end(), 0.0) / gpu_times.size();

		std::cout << "CPU : Avg : " << cpu_avg << " ms FPS : " << 1000.0 / cpu_avg << std::endl;
		std::cout << "GPU : Avg : " << gpu_avg << " ms FPS : " << 1000.0 / gpu_avg << std::endl;
	}

	return 0;
}