#include "Triangle.h"

/*TODO: Barycentric coordinates of 3D triangles- Refer to Section 2.7.2 of the textbook */
Vec3 Triangle::BaryCentric(const Vec3& point)
{
    //complete this function, see wiki    
    return Vec3(alpha, beta, gamma);
}


Vec3 Triangle::ComputeNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3)
{
    Vec3 v12 = p2 - p1;
    Vec3 v13 = p3 - p1;
    return v12.cross(v13);
}




