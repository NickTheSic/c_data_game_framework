#include <nl_math.h>

#ifdef NL_MATH_IMPLEMENTATION
#error incomplete
#else

#include <iostream>

void 
CreateOrtho(mat4f& matrix, float left, float right, float bottom, float top, float near_z, float far_z)
{
    matrix = glm::ortho<float>(left, right, bottom, top, near_z, far_z);
}

#endif
