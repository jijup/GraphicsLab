#include <Eigen/Dense>
#include <Eigen/Geometry>
#include<math.h>

#include "Image.h"

//Data types
typedef float Scalar;
typedef Eigen::Matrix<Scalar, 3, 1> Vec3;
typedef Eigen::Matrix<Scalar, 2, 1> Vec2;
typedef	unsigned char uchar;

using Colour = cv::Vec3b;

//OpenCV uses BGR order, but we follow RGB order. So if we ask OpenCV to color using red(), it shows blue.
//OpenCV image is in the form of matrix and follows row x cols, row major order. We use x along horizontal (cols) and y along vertical(rows). So, to color pixel (x, y), you may want to use (y, x).
Colour red() { return Colour(255, 0, 0); }
Colour green() { return Colour(0, 255, 0); }
Colour blue() { return Colour(0, 0, 255); }

struct Triangle {
    Vec3 v1, v2, v3;
};

struct Line {
    Vec3 v1, v2;
};

float triangleArea(Vec3 v1, Vec3 v2, Vec3 v3) {
	//See GRaphicsLab wiki page to complete the following
    Vec3 AB = ------;
    Vec3 AC = ------;
    return ----------;
}

void rasterize(Triangle t, Image &image) {

    // Vectors projected onto z=0 plane. y coordinates have been subtracted from 'image.rows' because OpenCV/raster images treat top left corner as the (0, 0).
    Vec3 s1 = Vec3(t.v1(0), image.rows-t.v1(1), 0);
    Vec3 s2 = Vec3(t.v2(0), image.rows-t.v2(1), 0);
    Vec3 s3 = Vec3(t.v3(0), image.rows-t.v3(1), 0);

    /// OPTIONAL: Restrict these bounds to only rasterize the triangle bounding box (optimization)
    int iMin = 0;
    int iMax = image.cols;
    int jMin = 0;
    int jMax = image.rows;

    float total_area = triangleArea(s1, s2, s3);

    for (int i = iMin; i < iMax; i++) {
		for (int j = jMin; j < jMax; j++) {

            // Location of fragment in image space            
            Vec3 pt = Vec3(i, j, 0.0);            

            //Task 2: Calculate barycentric coordinates of the fragment within current triangle. Use triangle areas for Barycentric coordinate computation. Refer to the slides for details.
            float alpha = -----;
			float beta = ------;
            float gamma = -----;
            if (std::abs( (alpha + beta + gamma) - 1) < 1e-5)
            {
                Colour pixel = red() * alpha + green() * beta + blue() * gamma;
                image(j, i) = cv::Vec3b(pixel[0], pixel[1], pixel[2]);
            }            


        }
    }

}

float LineEquation(const Vec3 & p0, const Vec3 & p1, float x, float y)
{
    //compute the implicit line equation -- four terms
    //(y0 - y1) x + (x1 - x0) y + x0y1 -x1y0
    float result = 0;
    result += (p0[1] - p1[1]) * x;
    result += -------; //(x1 - x0) y
    result += -------; //x0y1
    result += - (p1[0] * p0[1]);
    return result;

}

void ImageCordBound(int & xCord, int & yCord, int cols, int rows)
{
    if (xCord < 0) xCord = 0;
    if (xCord >= cols) xCord = cols - 1;
    if (yCord < 0) yCord = 0;
    if (yCord >= rows) yCord = rows -1;
}
void rasterize(Line l, Image &image) {

    // Vectors projected onto z=0 plane
    Vec3 s1 = Vec3(l.v1(0), 500-l.v1(1), 0);
    Vec3 s2 = Vec3(l.v2(0), 500-l.v2(1), 0);

    /// Task 1: Implement Bresenham's mipoint algorithm. 
	//TODO: Calculate error delta 
    float deltax = ------;
    float deltay = ------;

    if (deltax > deltay)
    {
        //going the x-direction
        //pre-processing the start and end points
        if (s1[0] > s2[0])
        {
            //swap
            Vec3 temp = s1;
            s1 = s2;
            s2 = temp;
        }
        int pixelStartX = (int)(s1[0]);
        int pixelStartY = (int)(s1[1]);
        ImageCordBound(pixelStartX,pixelStartY, image.cols, image.rows);

        int pixelEndX = (int)(s2[0]);
        int pixelEndY = (int)(s2[1]);
        ImageCordBound(pixelEndX,pixelEndY, image.cols, image.rows);


        //TODO: the Bresenham's/midpoint algorithm 
        int y = pixelStartY;
		//Calculate the d value
		float d = -----; //see wiki
        int yDelta = (pixelEndY - pixelStartY) > 0 ? 1 : -1;
        for (int x = pixelStartX; x < pixelEndX; ++ x)
        {
            
           //TODO: Complete this part. Color the pixel (y, x) with red. OpenCV BGR order, we follow RGB-->so blue() will display red()
		   image(y, x) = ------;
		   //increment y based on the midpoint check- use the d value to check whether the midpoint lies above or below the line (Refer to the slides)
		   if (d < 0)
            {
                y += -------;
                d += -------;
            }
            else {
                d -= --------;
            }
        }
    }
    else{
        //going the y-direction
        //pre-processing the start and end points
        if (s1[1] > s2[1])
        {
            //swap
            Vec3 temp = s1;
            s1 = s2;
            s2 = temp;
        }
        int pixelStartX = (int)(s1[0]);
        int pixelStartY = (int)(s1[1]);
        ImageCordBound(pixelStartX,pixelStartY, image.cols, image.rows);

        int pixelEndX = (int)(s2[0]);
        int pixelEndY = (int)(s2[1]);
        ImageCordBound(pixelEndX,pixelEndY, image.cols, image.rows);

        //the midpoint algorithm (15%)
        int x = pixelStartX;
		//flat d = ?
        int xDelta = (pixelEndX - pixelStartX) > 0 ? 1 : -1;
        for (int y = pixelStartY; y < pixelEndY; ++ y)        {
            
			//TODO: Complete this part. Color the pixel (y, x) with red. OpenCV BGR order, we follow RGB-->so blue() will display red()
			image(y, x) = ------;
		   //increment x based on the midpoint check- use the d value to check whether the midpoint lies above or below the line (Refer to the slides)
		   if (d < 0)
            {
                x += ---------;
                d += ---------;
            }
            else {
                d += -------;
            }
            
        }
    }


}

int main(int, char**){

    int wResolution = 500;
    int hResolution = 500;
	
	//Create an empty image
    Image image(wResolution, hResolution);

    //Triangle
    Triangle t1 = {
        Vec3(50,50,0), Vec3(250,400,0), Vec3(450,50,0)
    };    

   
    ////Lines
    Line l = {
        Vec3(50, 50, 0), Vec3(450, 50, 0)
    };

    //Task 3: Draw a grid with cells of size 20 x 20 pixels and render a triangle on the grid.
	
    //TODO:Horizontal lines    
        for(int y=50; y<=450;y=y+10){               
                //complete this code
				//rasterize the line
				//change y coorinates, e.g., l.v1=Vec3(50, y, 0);
        }
    
    //TODO:Vertical lines 
        for (int x = 50; x <= 460; x = x + 10) {
			//complete this code
			//rasterize the line
			//change x coorinates, e.g., l.v2=Vec3(x, 450, 0);
            
        }

    //TODO:Rasterize the triangle.  
	
	//save and display the rasterized image 
    image.save("./result.png");
    image.display();

    return EXIT_SUCCESS;
}
