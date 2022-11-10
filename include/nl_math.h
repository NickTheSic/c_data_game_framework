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
};

struct v3f
{
    float x,y,z;
};

struct mat4f
{
    float m11; float m12; float m13; float m14;
    float m21; float m22; float m23; float m24; 
    float m31; float m32; float m33; float m34; 
    float m41; float m42; float m43; float m44;
};

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

void CreateIdentityMartix(mat4f& matrix);
void CreateOrtho(mat4f& matrix, float left, float right, float bottom, float top, float near_z, float far_z);

#endif //NL_MATH_H
