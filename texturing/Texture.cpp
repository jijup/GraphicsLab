#include "Texture.h"

int Texture::clamp_w(int val) {
    if (val >= width) return width-1;
    else if (val < 0) return 0;
    else return val;
}

int Texture::clamp_h(int val) {
    if (val >= height) return height-1;
    else if (val < 0) return 0;
    else return val;
}


int Texture::bound(int val) {
    if (val > 255) return 255;
    else if (val < 0) return 0;
    else return val;
}


//look up based on simple round off
cv::Vec3b Texture::texture_lookup(const Vec2& uv) {
    float uv0 = (float)(uv[0] * width-0.5);
    float uv1 = (float)(uv[1] * height-0.5);
    int i = floor(uv0);
    int j = floor(uv1);
    i = clamp_w(i);
    j = clamp_h(j);
    cv::Vec3b pixelColor = get_pixel(i, j);
    pixelColor[0] = bound(pixelColor[0]);
    pixelColor[1] = bound(pixelColor[1]);
    pixelColor[2] = bound(pixelColor[2]);
    return pixelColor;
}


//look up based on bilinear interpolation
cv::Vec3b Texture::texture_lookup_bilinear(const Vec2& uv) {
    float uv0 = -----------;
    float uv1 = -----------;
	
    int i0 = floor(uv0);
    int j0 = floor(uv1);

    //Bound the values between 0 and height/width
    i0 = clamp_w(i0);
    j0 = clamp_h(j0);

    //Next pixels in u and v directions
    int i1 = (i0 < height - 1) ?  i0 + 1 : i0;   
    int j1 = (j0 < width - 1) ?  j0 + 1 :  j0;
    
    //Parameters for interpolation
    float au =  ----------;
    float av =  ----------;    
    float bu = 1.0f - au;
    float bv = 1.0f - av;

    //Get the texture colrs of the four pixels    
    cv::Vec3b pixel00 = get_pixel(i0, j0);
    cv::Vec3b pixel01 = get_pixel(i0, j1);
    cv::Vec3b pixel10 = get_pixel(i1, j0);
    cv::Vec3b pixel11 = get_pixel(i1, j1);


    //Find the interpolated color
    cv::Vec3b pixelColor;    
    pixelColor[0] = -------------------------;
    pixelColor[1] = -------------------------;
    pixelColor[2] = -------------------------;
    
    return pixelColor;
}

cv::Vec3b Texture::get_pixel(int i, int j)
{
    //std::cout << i << " " << j << std::endl;
    assert(i >= 0 && i < width);
    assert(j >= 0 && j < height);
    
    cv::Vec3b pixel;
    pixel[2] = (int)image.at(j * width*4 + 4 * i);  //RGB to BGR
    pixel[1] = (int)image.at(j * width*4 + 4 * i+1);
    pixel[0] = (int)image.at(j * width*4 + 4 * i+2);

    return pixel;
}




void Texture::loadTexture(const char* filename) {
    // Used snippet from https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
    unsigned width, height;
        //decode
    unsigned error = lodepng::decode(image, width, height, filename);
    this->height = height;
    this->width = width;
    
    //if there's an error, display it
    if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture

    // unfortunately they are upside down...lets fix that
    unsigned char* row = new unsigned char[4 * width];
    for (int i = 0; i < int(height) / 2; ++i) {
        memcpy(row, &image[4 * i * width], 4 * width * sizeof(unsigned char));
        memcpy(&image[4 * i * width], &image[image.size() - 4 * (i + 1) * width], 4 * width * sizeof(unsigned char));
        memcpy(&image[image.size() - 4 * (i + 1) * width], row, 4 * width * sizeof(unsigned char));
    }
    delete row;
}
