/* Starter Code for Exercise 2 of CSCI 4471/6671: Computer Graphics
Please refer to the corresponding wiki page and read the instructions carefully
 while completing the code.*/
 
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "Image.h"

//Data types
typedef Eigen::Matrix<float, 3, 1> Vec3;
typedef Eigen::Matrix<float, 2, 1> Vec2;

//Color functions
using Colour = cv::Vec3b; // BGR Value: Type aliasing (Colour is just an alias for cv:Vec3b type)
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }


//bounding the channel wise pixel color between 0 to 255
unsigned char Clamp(int pixelCol)
{
    if (pixelCol < 0) return 0;
    if (pixelCol >= 255) return 255;
    return pixelCol;
}



int main(int, char**){
	
	//Create an image object with 500 x 500 resolution.
    Image image = Image(500, 500);
	
	//Coordinates of image rectangle
	Vec3 llc = Vec3(-1, -1, -1);//lower left corner
	Vec3 urc = Vec3(1, 1, -1);//upper right corner
    int width = urc(0) - llc(0); //get width of the image plane
    int height = urc(1) - llc(1); //get the height of the image plane
    Vec2 pixelUV = Vec2((float)width / image.cols, (float)height / image.rows); //get the pixel spacing
	
	/// TODO: specify the camera position (cameraPoint) 
    

    ///TODO: Define the sphere: spherePos, sphereRadius (see wiki)
	// example: Vec3 SpherePos = Vec3(0, 0, -5);
	
	
	///TODO: specify the reflection coefficients of sphere, k_a, k_d, k_s, see wiki page
	
	
	///TODO: specify the lighting source 
	//example: Vec3 lightSource(1.0, -2.0, 3.0);
	
	
	///TODO: specify the ambient, diffuse and specular light colors, see wiki page
	//example: Vec3 diffLight(1.0f,1.0f, 1.0f);	
   
   
    Vec3 Origin = cameraPoint;
    //end

    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
			///TODO: Build primary rays. 
			//Find the pixel position (pixelPos) for each row(i) and col(j) and then construct the ray direction (pixelPos-Origin)
			
			//Normalize the ray, how?
           

            ///TODO: Ray-sphere intersection...(refer to the lecture slides and Section 4.4.1 of the textbook) 
			
			//Build the coefficients of the quadratic equation, A, B & C
           
			
			//Compute the discriminant
            
			
			
			//if the discriminant is greater than zero
			if(discriminant > 0){
            
			
				//find roots t1 and t2
                
				
                //determine which one is the real intersection point, the closest one min(t1, t2)
                

                if (t > 0){
                    ///TODO: Blinn-Phong shading 
					//Shade the pixel, normal is Intersection - SphereCenter, LightVector is lightSource- Intersection, make sure to normalize these vectors
                    
					//Compute difffuse term (diffuseterm)
                    float diffuseTerm = LightVector.dot(Normal);
                    if (diffuseTerm < 0) diffuseTerm = 0;
					
					//compute the half vector (HalfVec) between light vector and view vector (opposite of the ray direction)
					
					//Normalize the half vector
					
					//Compute specular term (specularTerm), similar to diffuseTerm, but use halfVec and Normal
					
					
					Colour colour(0, 0, 0); 
                    colour[0] = Clamp(((k_a[0] * lightAmbient[0] + k_d[0] * lightDiffuse[0] * diffuseTerm +k_s[0] * lightSpecular[0] * pow(specTerm, 1000)) * objectColor[0])*255); //change the value of shininess from 32 to 64, 128, 254 etc. and generate the images
                    colour[1] = --------;
                    colour[2] = --------;
                    image(i, j) = colour;
                } else {
					//The camera is inside the sphere?
                    image(i, j) = black();
                }
			
            } else {
                //No intersection, discriminant < 0 (background)
                image(i, j) = white();
            }

       }
    }
    ///Program should compile, run and generate the correct results  
    ///	Required outputs: (1) Ray traced image of a sphere (2) Ray traced image when the camera is placed inside the sphere (complete black)
    image.save("./result.png");
    image.display();

    return EXIT_SUCCESS;
}

