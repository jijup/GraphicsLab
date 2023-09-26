#include "Image.h"

cv::Vec3b& Image::operator()(int row, int col)
{
	assert(row >= 0 && row < rows);
	assert(col >= 0 && col < cols);
	return image.at<cv::Vec3b>(cv::Point(col, row));
}
void Image::display() {
	/// Display the image
	cv::imshow("image", image);
	cv::waitKey(0);
}

void Image::save(const std::string& filename) {
	cv::imwrite(filename, image);
}