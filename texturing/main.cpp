/*Triangle Texturing*/

#include "Triangle.h"
#include "Image.h"
#include "Texture.h"
#include<cstdlib>
#include<list>

using namespace std;

////Color functions
using Colour = cv::Vec3b; // RGB Value
Colour red() { return Colour(255, 0, 0); }
Colour green() { return Colour(0, 255, 0); }
Colour blue() { return Colour(0, 0, 255); }
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }

int main(int, char**) {

	Image image = Image(512, 512);

	Vec3 llc = Vec3(-1, -1, -1);
	Vec3 urc = Vec3(1, 1, -1);
	int width = urc(0) - llc(0);
	int height = urc(1) - llc(1);
	Vec2 pixelUV = Vec2((float)width / image.cols, (float)height / image.rows);


	Vec3 CameraPoint(0, 0, 0);

	//create the floor--use two triangles as follows. Note that the texture coordinates of triangle vertices are specified as well.
	list<Triangle>  tri_list{ Triangle(Vec3(-1, -1, -1), Vec3(1, -1, -1), Vec3(-1, -1, -5), Vec2(0, 0), Vec2(1, 0), Vec2(0,1)),
		Triangle(Vec3(1, -1, -1), Vec3(1, -1, -5), Vec3(-1, -1, -5), Vec2(1, 0), Vec2(1, 1), Vec2(0, 1)) };

	float epsilon = 0.00001;

	Vec3 Origin = CameraPoint;

	//Texture
	Texture tex = Texture();
	//Load texture. Make sure you have the Texture folder containing the texture images in your exe directory
	tex.loadTexture("Textures/water.png");


	float u, v, w;
	for (int row = 0; row < image.rows; ++row) {

		for (int col = 0; col < image.cols; ++col) {

			Colour pixelColor = Colour(255, 255, 255);

			u = llc(0) + pixelUV(0) * (col + 0.5);
			v = llc(1) + 2 - pixelUV(1) * (row + 0.5);
			w = -1;

			Vec3 pixelPos = Vec3(u, v, w);

			Vec3 Direction = pixelPos - Origin;
			//Vec3 Direction = Vec3(0, 0, -1);
			Direction.normalize();
			
			//loop to go over the primitives in the scene, i.e.,triangles
			for (auto it = tri_list.begin(); it != tri_list.end(); ++it) {

				//Task 1: Triangle shading
				//Ray-plane intersection...use the triangle normal (triangle.normal_), normalize it, refer to Section 4.4.3 of the textbook
				Vec3 tNormal = it->normal_;
				tNormal.normalize();
				Scalar t = (float)(it->vertex1_ - Origin).dot(tNormal) / Direction.dot(tNormal);


				//Ray-triangle intersection...compute the intersection point of the ray-plane intersection
				Vec3 Intersection = Origin + t * Direction;
				Vec3 baryCoords = it->BaryCentric(Intersection);

				//Does the intersection point lies in the triangle? Use barycentric coordinates for testing
				if (fabs(baryCoords.sum() - 1) < epsilon && (baryCoords[0] <= 1 && baryCoords[0] >= 0) &&
					(baryCoords[1] <= 1 && baryCoords[1] >= 0) && (baryCoords[2] <= 1 && baryCoords[2] >= 0)) {
					//if yes, plainly shade the pixel

					//simple round off based texture mapping. Comment out the following statement when you do bilinear interpolation
					pixelColor = tex.texture_lookup(it->getTextCoord(Intersection));
					

					//Uncomment the below statement for bilinear interpolation
					//pixelColor = tex.texture_lookup_bilinear(it->getTextCoord(Intersection));
					
				}


			}

			image(row, col) = pixelColor;

		}

	}
	//system("pause");
	image.save("./result.png");
	image.display();


	return EXIT_SUCCESS;
}

