#include <nl_shader.h>
#include <iostream>
#include <nl_gl.h>
#include <nl_math.h>
#include <nl_renderer.h>

// TODO: Put in cpp file only
static int GetUniformLocation(Shader* shader, std::string name)
{
    auto it = shader->uniform_locs.find(name);

    if (it == shader->uniform_locs.end())
    {
        shader->uniform_locs[name] = glGetUniformLocation(shader->program, name.c_str());
    }

    return shader->uniform_locs[name];
}

void SetUniform(Shader* shader, const char* name, float val)
{
    int loc = GetUniformLocation(shader, name);
    glUniform1f(loc, val);
}

void SetUniform(Shader* shader, const char* name, const mat4f& val)
{
    int loc = GetUniformLocation(shader, name);
    glUniformMatrix4fv(loc,1, GL_FALSE, glm::value_ptr(val));
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
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Error compiling shader: " << type << std::endl;
        std::cout << "\t" << infoLog << std::endl;
    }
}

void 
CompileSpriteShaderProgram(Renderer* renderer)
{
    unsigned int vertShader, fragShader;
    
    const char* vertCode = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 inPos;"
        "layout (location = 1) in vec2 inCoords;"
        "out vec2 TexCoords;"
        "uniform mat4 view;"
        "void main(){"
        "gl_Position = view * vec4(inPos, 1.0f);"
        "TexCoords = inCoords;"
        "}\0"
        ;
    
    const char* fragCode = 
        "#version 330 core\n"
        "out vec4 FragColor;"
        "in vec2 TexCoords;"
        "uniform sampler2D tex;"
        "void main(){"
        "FragColor = texture(tex, TexCoords);"
        "if (FragColor.a == 0) discard;"
        "}\0"
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
