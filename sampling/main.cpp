/*Antialiasing using Jittering/Stratified Supersampling*/


#include "Triangle.h"
#include "Image.h"


//Color functions
using Colour = cv::Vec3b; // RGB Value
Colour red() { return Colour(255, 0, 0); }
Colour green() { return Colour(0, 255, 0); }
Colour blue() { return Colour(0, 0, 255); }
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }

uchar Clamp(int color)
{
	if (color < 0) return 0;
	if (color >= 255) return 255;
	return color;
}


int main(int, char**) {

	Image image = Image(500, 500);

	Vec3 llc = Vec3(-1, -1, -1);
	Vec3 urc = Vec3(1, 1, -1);
	int width = urc(0) - llc(0);
	int height = urc(1) - llc(1);
	Vec2 pixelUV = Vec2((float)width / image.cols, (float)height / image.rows);


	Vec3 CameraPoint(0, 0, 0);
	Triangle triangle = Triangle(Vec3(-1, -1, -1), Vec3(1, -1, -1), Vec3(0, 0.75, -1));

	Vec3 LightSource(0, 1, 1);

	Vec3 ambient(255, 0, 0);

	//useful for checking whether the intersection point is inside or outside the triangle
	float epsilon = 0.00001;

	Vec3 Origin = CameraPoint;
	
	//include the h_samples and v_samples of your supersampling grid, e.g., the grid may be 4 x 4 or 8 x 8
	int h_samples = 0;
	int v_samples = 0;
	//end

	for (int row = 0; row < image.rows; ++row) {
		for (int col = 0; col < image.cols; ++col) {
			Vec3 pixelColour(0, 0, 0);
			//Include the loops for supersampling for Task 2.	
			
					//Find the pixelPos.
					
					Vec3 Direction = pixelPos - Origin;
					Direction.normalize();

					//Task 1: Triangle shading 
					//Ray-plane intersection...use the triangle normal (triangle.normal_), normalize it, refer to Section 4.4.3 of the textbook and finf the t
					


					//Ray-triangle intersection...compute the intersection point of the ray-plane intersection
					
					
					//Compute the Barycentric coordinates
					

					//Does the intersection point lies in the triangle? Use barycentric coordinates for testing. Use epsilon to avoid rounding off errors. refer to the slides for the condition.
					//Suem of the Barycentric coordinates must be 1 and each coordinate must lie between 0 and 1
					if () {
						
						//if yes, add the sample color to pixelColour for averaging at the end. Use the object colour.	

						

					}
					else {
						pixelColour +=Vec3(255, 255, 255);						
						
					}
					
				}

			}
			//TODO:Compute the average of pixelColour for super sampling			

			image(row, col)=Colour(pixelColour[0], pixelColour[1], pixelColour[2]);

		}

	}


	image.save("./result.png");
	image.display();

	return EXIT_SUCCESS;
}

