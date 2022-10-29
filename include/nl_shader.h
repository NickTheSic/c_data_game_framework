
#ifndef _NL_SHADER_H_
#define _NL_SHADER_H_

#include <map>
#include <string>
#include <nl_math.h>

struct Shader
{
    unsigned int program;
    std::map<std::string, unsigned int> uniform_locs; 
};

void SetUniform(Shader* shader, const char* name, float val);
void SetUniform(Shader* shader, const char* name, const mat4f& val);

void CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderCode);
void CompileSpriteShaderProgram(struct Renderer* renderer);

#endif //_NL_SHADER_H