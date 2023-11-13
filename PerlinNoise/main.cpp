#include "Image.h"
#include<cstdlib>
#include<cmath>
#include "Perlin.h"


const int width=512, height=512;

//Color functions
using Colour = cv::Vec3b; 

int main(int, char**) {

	Image image = Image(width, height);
	Perlin Noise = Perlin(width,height);

	float noise;
	int period = 32; //TODO4: Change the period to 64 and 128 and generate the noise.
	float frequency = (float)1 / period;
		
	for (int j = 0; j < image.rows; ++j) {
		for (int i = 0; i < image.cols; ++i) {

			noise = Noise.noise(i*frequency, j*frequency, 0); 

			//Maping [-1, 1] to [0, 1]
			noise = (noise + 1) * 0.5;
			
			//0-1 to 0-255 based gray scale
			int color = round(255 * noise);	
			image(j, i) = Colour(color, color, color);

		}

	}

	image.save("./result.png");
	image.display();

	return EXIT_SUCCESS;
}

