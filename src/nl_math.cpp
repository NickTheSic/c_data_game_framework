#include <nl_math.h>

#ifdef NL_MATH_IMPLEMENTATION

void CreateIdentityMatrix(mat4f& mat)
{
    mat.m11 = 1; mat.m12 = 0; mat.m13 = 0; mat.m14 = 0;
    mat.m21 = 0; mat.m22 = 1; mat.m23 = 0; mat.m24 = 0;
    mat.m31 = 0; mat.m32 = 0; mat.m33 = 1; mat.m34 = 0;
    mat.m41 = 0; mat.m42 = 0; mat.m43 = 0; mat.m44 = 1;
}

void
CreateOrtho(mat4f& mat, float left, float right, float bottom, float top, float near_z, float far_z)
{
    float delta_x = right-left;
    float delta_y = top-bottom;
    float delta_z = far_z-near_z;

    mat.m11 = 2.0f/delta_x;
    mat.m12 = mat.m13 = mat.m14 = 0.0f;
    
    mat.m22 = 2.0f/delta_y;
    mat.m21 = mat.m23 = mat.m24 = 0.0f;

    mat.m33 = -2.0f/delta_z;
    mat.m31 = mat.m32 = mat.m34 = 0.0f;

    mat.m41 = -(right + left) / delta_x;
    mat.m42 = -(top + bottom) / delta_y;
    mat.m43 = -(near_z) / delta_z;
    
    mat.m44 = 1;
}

#else

#include <iostream>

void
CreateIdentityMartix(mat4f& mat)
{
    mat = glm::mat4(1.0f);
}

void 
CreateOrtho(mat4f& matrix, float left, float right, float bottom, float top, float near_z, float far_z)
{
    matrix = glm::ortho<float>(left, right, bottom, top, near_z, far_z);
}

#endif
