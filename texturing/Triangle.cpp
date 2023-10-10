#include "Triangle.h"

/*Task 1: Barycentric coordinates of 3D triangles- Refer to Section 2.7.2 of the textbook */
Vec3 Triangle::BaryCentric(const Vec3& point)
{
    Vec3 na = ComputeNormal(vertex2_, vertex3_, point);
    Vec3 nb = ComputeNormal(vertex3_, vertex1_, point);
    Vec3 nc = ComputeNormal(vertex1_, vertex2_, point);

    Scalar norm = normal_.squaredNorm();
    Scalar alpha = normal_.dot(na) / norm;
    Scalar beta = normal_.dot(nb) / norm;
    Scalar gamma = normal_.dot(nc) / norm;   
    
    return Vec3(alpha, beta, gamma);
}


Vec2 Triangle::getTextCoord(const Vec3& point) {
    Vec3 baryCoords = BaryCentric(point);

    Vec2 uv;

    //Interpolate the triangle vertex texture coordinates using Barycentric coordinates to get the 'point' texture values
    uv[0] = ----------------;
    uv[1] = ----------------;  
        
    return uv;
}



Vec3 Triangle::ComputeNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3)
{
    Vec3 v12 = p2 - p1;
    Vec3 v13 = p3 - p1;
    return v12.cross(v13);
}




