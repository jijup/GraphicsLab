#pragma once
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include<math.h>
#include<iostream>

//Data types
typedef float Scalar;
typedef Eigen::Matrix<Scalar, 3, 1> Vec3;
typedef Eigen::Matrix<Scalar, 2, 1> Vec2;
typedef	unsigned char uchar;

class Triangle
{
public:
	Triangle() {};
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3) :vertex1_(v1), vertex2_(v2), vertex3_(v3) {		
		normal_ = ComputeNormal(v1, v2, v3);
	};
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec2 t1, Vec2 t2, Vec2 t3) :vertex1_(v1), vertex2_(v2), vertex3_(v3), text_coord1(t1), text_coord2(t2), text_coord3(t3) {
		normal_ = ComputeNormal(v1, v2, v3);
	};

	Vec3 BaryCentric(const Vec3&);		
	Vec3 ComputeNormal(const Vec3&, const Vec3&, const Vec3&);
	Vec2 getTextCoord(const Vec3&);

public:
	Vec3 vertex1_, vertex2_, vertex3_;
	Vec2 text_coord1, text_coord2, text_coord3;
	Vec3 normal_;
	
};

