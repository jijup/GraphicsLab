#pragma once
#include "opencv2/core/core.hpp"       
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

class Image
{
public:
	Image() {};
	Image(unsigned xRes, unsigned yRes) :cols(xRes), rows(yRes) {};

	cv::Vec3b& operator()(int row, int col);
	
	/// Displays the image and pauses program execution (until key pressed)
	void display();

	/// Use files with "*.png" extension
	void save(const std::string& filename);

public:
	/// Image resolution
	unsigned cols = 100;
	unsigned rows = 100;

	///Image matrix - 8 bit depth unsigned with 3 channels
	cv::Mat image = cv::Mat(rows, cols, CV_8UC3, cv::Scalar(255, 255, 255));

};

