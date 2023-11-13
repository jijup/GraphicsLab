#include "Perlin.h"


Perlin::Perlin(int w, int h) {
	width = w;
	height = h;
		
}

/// TODO1: Implement the linear interpolation 

float Perlin::lerp(float x, float y, float t) {
	/// TODO: Implement linear interpolation between x and y
	 
	return 0; //to be changed
}


/// TODO2: Implement the fade function (try both cubic as well as quintic polynomials and generatee the corresponding noise textures) 

float Perlin::fade(float t) {
	/// TODO2a: Implement the quintic function (optimized) 
    ///	TODO2b: Implement the cubic function 
	return 0;//to be changed	
}

//function returns one of the eight gradients
Vec2 Gradient(int index) {
	//index is the value from the permutation table
	int h = index & 7;
	if (h == 0)
		return  Vec2(1.0, 0.0);
	else if (h == 1)
		return Vec2(0.707, 0.707);
	else if (h == 2)
		return  Vec2(0.0, 1.0);
	else if (h == 3)
		return Vec2(-0.707, 0.707);
	else if (h == 4)
		return  Vec2(-1.0, 0.0);
	else if (h == 5)
		return Vec2(-0.707, -0.707);
	else if (h == 6)
		return Vec2(0.0, -1.0);
	else
		return Vec2(0.707, -0.707);
		
}

//TODO3: Complete the following noise generation function. The function takes three coordinates of the sample point and returns the noise vlaue at that point. 
//Since this is 2D Noise, we keep z=0 for all the points and use x and y coordinates to generate the noise value.

float Perlin::noise(float xCoord, float yCoord, float zCoord) {	

	// TODO: Unit cube vertex coordinates surrounding the sample point. In this example find the unit square vertex coordinates in X and Y coords. See Graphics lab wikipedia.
	int X0 = --------;
	int X1 = --------;
	int Y0 = --------;
	int Y1 = --------;
	int Z0 = --------;
	int Z1 = --------;

	// Determine sample point position within unit cube (unit square in our case) 
	float pointX0 = -------;
	float pointX1 = -------;
	float pointY0 = -------;
	float pointY1 = -------;
	float pointZ0 = -------;
	float pointZ1 = -------;

	//Construct distance vectors from each corner to the sample point
    Vec2 VX0Y0(----, ----);
	Vec2 VX1Y0(----, ----);
	Vec2 VX0Y1(----, ----);
	Vec2 VX1Y1(----, ----); 
	
	//Fetch the gradients at corners 
	int gradientIndex;
	gradientIndex = -----;
	Vec2 g00 = ------;
	gradientIndex = -----;
	Vec2 g10 = -----;
	gradientIndex = -----;
	Vec2 g01 = -----;
	gradientIndex = -----;
	Vec2 g11 = -----;

	//TODO: Get scalars at corners HINT: take the dot product of gradient and the distance vectors. 
	float dotX0Y0Z0, dotX0Y0Z1, dotX0Y1Z0, dotX0Y1Z1, dotX1Y0Z0, dotX1Y0Z1, dotX1Y1Z0, dotX1Y1Z1;
	dotX0Y0Z0 = -----;	
	dotX1Y0Z0 = -----;	
	dotX0Y1Z0 = -----;	
	dotX1Y1Z0 = -----;

	//TODO: Interpolate along X 
	float st = -----;
	float uv = -----;

	//TODO: Interpolate along Y 
	float result = -----;


	// Return the result
	return result;	
	
}
