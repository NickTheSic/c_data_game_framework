/* date = October 22nd 2022 2:34 pm */

#ifndef NL_MATH_H
#define NL_MATH_H

// if WEB define the define that removes instrinsics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct v2i
{
    int x, y;
};

typedef glm::vec2 v2f;
typedef glm::vec3 v3f;
typedef glm::mat4 mat4f;

#endif //NL_MATH_H
