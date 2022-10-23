#include <Renderer.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

struct GeneratedSprite
{
    int x,y,channel;
    unsigned char* data;
};

void 
InitializeSpriteSheet(SpriteSheet *sheet, int sx, int sy)
{
    sheet->atlasSize.x = sx;
    sheet->atlasSize.y = sy;
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &sheet->textureID);
    glBindTexture(GL_TEXTURE_2D, sheet->textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GeneratedSprite gsd = {};
    gsd.data = stbi_load("data/test.png", &gsd.x, &gsd.y, &gsd.channel,4);
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 sheet->atlasSize.x,
                 sheet->atlasSize.y,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 gsd.data
                 );
    
    stbi_image_free(gsd.data);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexData), (void*)(offsetof(SpriteVertexData, position)));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexData), (void*)(offsetof(SpriteVertexData, uv)));
    glEnableVertexAttribArray(1);
    
    //glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SpriteVertexData), (void*)(offsetof(SpriteVertexData, colour)));
    //glEnableVertexAttribArray(2);
    
}

bool
LoadSprite(SpriteSheet* sheet, Sprite* sprite, const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    GeneratedSprite gsd = {};
    
    gsd.data = stbi_load(path, &gsd.x, &gsd.y, &gsd.channel, 4);
    
    if (stbi_failure_reason())
    {
        std::cout << stbi_failure_reason() << std::endl;
        return false;
    }
    
    if ((sheet->currentAtlasLoc.x + gsd.x) > sheet->atlasSize.x)
    {
        sheet->currentAtlasLoc.x = 0; 
        sheet->currentAtlasLoc.y = sheet->usedAtlasHeight;
    }
    
    if (sheet->currentAtlasLoc.y + gsd.y > sheet->atlasSize.y)
    {
        fprintf(stderr, "No more height in the current spritesheet to add sprite.\n");
        return false;
    }
    
    sprite->bl_coord.x = (float)sheet->currentAtlasLoc.x / (float)sheet->atlasSize.x;
    sprite->bl_coord.y = (float)sheet->currentAtlasLoc.y / (float)sheet->atlasSize.y;
    sprite->ur_coord.x = (float)(sheet->currentAtlasLoc.x + gsd.x) / (float)sheet->atlasSize.x;
    sprite->ur_coord.y = (float)(sheet->currentAtlasLoc.y + gsd.y) / (float)sheet->atlasSize.y;
    
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    sheet->currentAtlasLoc.x, sheet->currentAtlasLoc.y,
                    gsd.x, gsd.y, 
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    gsd.data);
    
    sheet->currentAtlasLoc.x += gsd.x;
    sheet->usedAtlasHeight = (sheet->usedAtlasHeight - sheet->currentAtlasLoc.y) < gsd.y
        ? gsd.y + sheet->currentAtlasLoc.y : sheet->usedAtlasHeight;
    
    stbi_image_free(gsd.data);
    
    return true;
}

void
AddSpriteToRender(SpriteSheet* sheet, Sprite* sprite)
{
    if (sheet->renderer.vertexCount + 4 > sheet->renderer.maxVertices)
    {
        EndRender(&sheet->renderer);
    }
    
    SpriteVertexData vertices[4] = {};
    v3f& pos0 = vertices[0].position;
    pos0.x = sprite->position.x;
    pos0.y = sprite->position.y;
    pos0.z = sprite->position.z;
    
    v3f& pos1 = vertices[1].position;
    pos1.x = sprite->position.x + sprite->size.x;
    pos1.y = sprite->position.y;
    pos1.z = sprite->position.z;
    
    v3f& pos2 = vertices[2].position;
    pos2.x = sprite->position.x + sprite->size.x;
    pos2.y = sprite->position.y + sprite->size.y;
    pos2.z = sprite->position.z;
    
    v3f& pos3 = vertices[3].position;
    pos3.x = sprite->position.x;
    pos3.y = sprite->position.y + sprite->size.y;
    pos3.z = sprite->position.z;
    
    v2f& uv0 = vertices[0].uv;
    uv0.x = sprite->bl_coord.x;
    uv0.y = sprite->bl_coord.y;
    
    v2f& uv1 = vertices[1].uv;
    uv1.x = sprite->ur_coord.x;
    uv1.y = sprite->bl_coord.y;
    
    v2f& uv2 = vertices[2].uv;
    uv2.x = sprite->ur_coord.x;
    uv2.y = sprite->ur_coord.y;
    
    v2f& uv3 = vertices[3].uv;
    uv3.x = sprite->bl_coord.x;
    uv3.y = sprite->ur_coord.y;
    
    glBufferSubData(GL_ARRAY_BUFFER,
                    sheet->renderer.vertexCount*sizeof(SpriteVertexData),
                    4*sizeof(SpriteVertexData),
                    &vertices[0]);
    
    sheet->renderer.vertexCount += 4;
    ++sheet->renderer.drawCount;
}

void EndRender(Renderer* renderer)
{
    glDrawElements(GL_TRIANGLES, renderer->drawCount * 6, GL_UNSIGNED_INT, 0);
    renderer->vertexCount = renderer->drawCount = 0;
}

void 
CleanupSpriteSheet(SpriteSheet* sheet)
{
    glDeleteTextures(1, &sheet->textureID);
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
        //"layout (location = 2) in vec4 inColour;"
        "out vec4 SpriteColour;"
        "out vec2 TexCoords;"
        "void main(){"
        "gl_Position = vec4(inPos, 1.0f);"
        //"SpriteColour = inColour;"
        "TexCoords = inCoords;"
        "}\0"
        ;
    
    const char* fragCode = 
        "#version 330 core\n"
        "out vec4 FragColor;"
        "in vec4 SpriteColour;"
        "in vec2 TexCoords;"
        "uniform sampler2D tex;"
        "void main(){"
        "FragColor = texture(tex, TexCoords);"
        //"* SpriteColour;"
        "}\0"
        ;
    
    CompileShaderCode(vertShader, GL_VERTEX_SHADER, vertCode);
    CompileShaderCode(fragShader, GL_FRAGMENT_SHADER, fragCode);
    
    renderer->shaderProgram = glCreateProgram();
    glAttachShader(renderer->shaderProgram, vertShader);
    glAttachShader(renderer->shaderProgram, fragShader);
    glLinkProgram(renderer->shaderProgram);
    
    // check program error
    {
        int  success;
        char infoLog[512];
        glGetProgramiv(renderer->shaderProgram, GL_LINK_STATUS, &success);
        
        if (!success)
        {
            glGetShaderInfoLog(renderer->shaderProgram, 512, NULL, infoLog);
            std::cout << "Error linking Shader Program" << std::endl;
            std::cout << infoLog << std::endl;
        }
    }
    
    glUseProgram(renderer->shaderProgram);
    
    glDeleteShader(fragShader);
    glDeleteShader(vertShader);
}

void 
InitializeRenderer(Renderer* renderer, unsigned int BatchCount, size_t DataSize)
{
    renderer->maxVertices = BatchCount*4;
    unsigned int IndiceCount = BatchCount*6;
    
    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glGenBuffers(1, &renderer->ebo);
    
    glBindVertexArray(renderer->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, renderer->maxVertices * DataSize, NULL, GL_DYNAMIC_DRAW);
    
    unsigned int* indices = new unsigned int[IndiceCount];
    unsigned int offset = 0;
    for (unsigned int i = 0; i < IndiceCount; i+=6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndiceCount*sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    delete[] indices;
}

void 
CleanupRenderer(Renderer* renderer)
{
    glDeleteBuffers(1, &renderer->ebo);
    glDeleteBuffers(1, &renderer->vbo);
    glDeleteVertexArrays(1, &renderer->vao);
    
    glDeleteProgram(renderer->shaderProgram);
}

