/* date = October 22nd 2022 2:34 pm */

#ifndef NL_MATH_H
#define NL_MATH_H

struct v2i
{
    int x, y;
};

#ifdef NL_MATH_IMPLEMENTATION
struct v2f
{
    float x, y;
}

struct v3f
{
    float x,y,z;
}

struct mat4f
{
    float m11, m12, m13, m14,
          m21, m22, m23, m24,
          m31, m32, m33, m34,
          m41, m42, m43, m44;
}

void CreateIdentityMatrix(mat4f& mat)
{
    mat.m11 = 1; mat.m12 = 0; mat.m13 = 0; mat.m14 = 0;
    mat.m21 = 0; mat.m22 = 1; mat.m23 = 0; mat.m24 = 0;
    mat.m31 = 0; mat.m32 = 0; mat.m33 = 1; mat.m34 = 0;
    mat.m41 = 0; mat.m42 = 0; mat.m43 = 0; mat.m44 = 1;
}

#else

#ifdef __EMSCRIPTEN__
 #define GLM_FORCE_PURE
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef glm::vec2 v2f;
typedef glm::vec3 v3f;
typedef glm::mat4 mat4f;

#endif

void CreateOrtho(mat4f& matrix, float left, float right, float bottom, float top, float near_z, float far_z);

#endif //NL_MATH_H
