#pragma once
#include "lodepng.h"
#include<iostream>
#include<vector>
#include "opencv2/core/core.hpp"       
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include<math.h>

//Data types
typedef Eigen::Matrix<float, 3, 1> Vec3;
typedef Eigen::Matrix<float, 2, 1> Vec2;

class Texture
{
public: 
	Texture() {};
	Texture(unsigned xRes, unsigned yRes) :width(xRes), height(yRes) {};
	void loadTexture(const char*);
	cv::Vec3b texture_lookup(const Vec2&);
	cv::Vec3b texture_lookup_bilinear(const Vec2&);
	cv::Vec3b get_pixel(int, int);
	int clamp_w(int);
	int clamp_h(int);	
	int bound(int);

public:
	unsigned int height = 100;
	unsigned int width = 100;
	std::vector<unsigned char> image;
};

