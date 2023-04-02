#include <nl_shader.h>
#include <iostream>
#include <nl_gl.h>
#include <nl_math.h>
#include <nl_renderer.h>

#if defined PLATFORM_WEB || defined __EMSCRIPTEN__
#define SHADER_VERSION_HEADER "#version 300 es\n precision mediump float;\n"

#elif defined (PLATFORM_WINDOWS) || defined(_WIN32)
#define SHADER_VERSION_HEADER  "#version 330 core\n"

#else
#error Double check the shaders required for glsl on the platform!
#endif

// Keeping around for reference.  I do like using a map and shader, but I don't need this for now
// In the future I could make my own map or come back to this, but for now I only have specific uniforms I use
//static int 
//GetUniformLocation(Shader* shader, std::string name)
//{
//    auto it = shader->uniform_locs.find(name);
//
//    if (it == shader->uniform_locs.end())
//    {
//        shader->uniform_locs[name] = glGetUniformLocation(shader->program, name.c_str());
//    }
//
//    return shader->uniform_locs[name];
//}
//
//void 
//SetUniform(Shader* shader, const char* name, float val)
//{
//    int loc = GetUniformLocation(shader, name);
//    glUniform1f(loc, val);
//}
//
//void 
//SetUniform(Shader* shader, const char* name, const mat4f& val)
//{
//    int loc = GetUniformLocation(shader, name);
//
//    #ifndef USE_GLM_MATH
//    glUniformMatrix4fv(loc,1, GL_FALSE, &val.m11);
//    #else
//    glUniformMatrix4fv(loc,1, GL_FALSE, glm::value_ptr(val));
//    #endif
//}

void 
SetViewUniform(Shader* shader, const mat4f& val)
{
    // TODO: Initialize this somewhere?  Don't need to check if set if we just set it elsewhere
    if (shader->view_uniform == -1)
    {
        // Could make "view" a constant somewhere in case I decide to change it
        shader->view_uniform = glGetUniformLocation(shader->program, "view");
    }

    glUniformMatrix4fv(shader->view_uniform, 1, GL_FALSE, &val.m11);

    // hypothetically, if I brought back the old way I can just call it directly here as so
    // SetUniform(&fw->shader, "view", fw->main_camera.view);
}

void
CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderCode)
{
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, 0);
    glCompileShader(shader);
    
    int  success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        LOG("Error Compiling shader: %d\n\t%s\n", type, info_log);
    }
}

void 
CompileSpriteShaderProgram(Renderer* renderer)
{
    unsigned int vertShader, fragShader;
    
    const char* vertCode = 
        SHADER_VERSION_HEADER
        "layout (location = 0) in vec3 inPos;    \n"
        "layout (location = 1) in vec2 inCoords; \n"
        "out vec2 TexCoords;                     \n"
        "uniform mat4 view;                      \n"
        "void main(){                            \n"
        "gl_Position = view * vec4(inPos, 1.0f); \n"
        "TexCoords = inCoords;                   \n"
        "}                                       \0"
        ;
    
    const char* fragCode = 
        SHADER_VERSION_HEADER
        "out vec4 FragColor;                     \n"
        "in vec2 TexCoords;                      \n"
        "uniform sampler2D text;                 \n"
        "void main(){                            \n"
        "FragColor = texture(text, TexCoords);   \n"
        //"if (FragColor.a == 0.0) discard;        \n"
        "}                                       \0"
        ;
    
    CompileShaderCode(vertShader, GL_VERTEX_SHADER, vertCode);
    CompileShaderCode(fragShader, GL_FRAGMENT_SHADER, fragCode);
    
    renderer->shader_program = glCreateProgram();
    glAttachShader(renderer->shader_program, vertShader);
    glAttachShader(renderer->shader_program, fragShader);
    glLinkProgram(renderer->shader_program);
    
    // check program error
    {
        int  success;
        char infoLog[512];
        glGetProgramiv(renderer->shader_program, GL_LINK_STATUS, &success);
        
        if (!success)
        {
            glGetShaderInfoLog(renderer->shader_program, 512, NULL, infoLog);
            std::cout << "Error linking Shader Program" << std::endl;
            std::cout << infoLog << std::endl;
        }
    }
    
    glUseProgram(renderer->shader_program);
    
    glDeleteShader(fragShader);
    glDeleteShader(vertShader);
}
