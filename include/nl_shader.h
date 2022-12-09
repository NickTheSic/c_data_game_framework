
#ifndef _NL_SHADER_H_
#define _NL_SHADER_H_

#include <map>
#include <string>
#include <nl_math.h>

struct Shader
{
    unsigned int program;

    int view_uniform = -1;
};

void SetViewUniform(Shader* shader, const mat4f& val);

void CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderCode);
void CompileSpriteShaderProgram(struct Renderer* renderer);

#endif //_NL_SHADER_H