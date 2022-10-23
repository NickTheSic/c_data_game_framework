#include <stdio.h>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <main.h>

void 
InitializeSpriteSheet(SpriteSheet *sheet)
{
    sheet->atlasSize.x = 1024;
    sheet->atlasSize.y = 1024;
    
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
    
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SpriteVertexData), (void*)(offsetof(SpriteVertexData, colour)));
    glEnableVertexAttribArray(2);
    
}

bool
AddSprite(SpriteSheet* sheet, Sprite* sprite, const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    GeneratedSprite gsd = {};
    
    gsd.data = stbi_load(path, &gsd.x, &gsd.y, &gsd.channel, 4);
    
    if (stbi_failure_reason())
    {
        std::cout << stbi_failure_reason() << std::endl;
        return false;
    }
    
    if (sheet->currentAtlasLoc.y + gsd.y > sheet->atlasSize.y)
    {
        fprintf(stderr, "No more height in the current spritesheet. But permanent\n");
        return false;
    }
    
    if ((sheet->currentAtlasLoc.x + gsd.x) > sheet->atlasSize.x)
    {
        sheet->currentAtlasLoc.x = 0; 
        sheet->currentAtlasLoc.y = sheet->usedAtlasHeight;
        if (sheet->currentAtlasLoc.y + gsd.y > sheet->atlasSize.y)
        {
            fprintf(stderr, "No more height in the current spritesheet\n");
            return false;
        }
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
        "layout (location = 2) in vec4 inColour;"
        "out vec4 SpriteColour;"
        "out vec2 TexCoords;"
        "void main(){"
        "gl_Position = vec4(inPos, 1.0f);"
        "SpriteColour = inColour;"
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
    for (unsigned int i = 0; i < BatchCount; i+=6)
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

int 
main()
{
    GLFWwindow* window = 0;
    
    // int glfw and other platform stuff
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        window = glfwCreateWindow(800,800,"TEST",0,0);
        if (window == 0)
        {
            fprintf(stderr, "Failed to create window");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            fprintf(stderr,"Failed to initialize glad\n");
            
            return -1;
        }
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH);
    glViewport(0, 0, 800, 800);
    glClearColor(0.1,0.2,0.2,1.0);
    
    // Custom Init
    SpriteSheet spriteSheet = {};
    {
        InitializeRenderer(&spriteSheet.renderer, 8, sizeof(SpriteVertexData));
        InitializeSpriteSheet(&spriteSheet);
        CompileSpriteShaderProgram(&spriteSheet.renderer);
    }
    
    int keyState = glfwGetKey(window, GLFW_KEY_E);
    
    int index = 1;
    const char* files[] =
    {
        "data/blue64.png",
        "data/red256.png",
        "data/white256x128.png",
        "data/blue64.png",
        "data/green128x256.png",
        "data/blue64.png",
        "data/red256.png",
    };
    
    std::vector<Sprite> Sprites;
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        int state = glfwGetKey(window, GLFW_KEY_E);
        if (state != keyState)
        {
            keyState = state;
            if (state == GLFW_PRESS)
            {
                //fprintf(stderr,"E Key Pressed\n");
                index%=7;
                {
                    Sprite spr = {};
                    if (AddSprite(&spriteSheet, &spr, files[index]))
                    {
                        //fprintf(stderr, "Adding sprite\n");
                        Sprites.push_back(spr);
                    }else{
                        fprintf(stderr, "Failed to add sprite\n");
                    }
                    ++index;
                }
            }
        }
        
        // custom render code
        {
            glUseProgram(spriteSheet.renderer.shaderProgram);
            glBindVertexArray(spriteSheet.renderer.vao);
            glBindBuffer(GL_ARRAY_BUFFER, spriteSheet.renderer.vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spriteSheet.renderer.ebo);
            glBindTexture(GL_TEXTURE_2D, spriteSheet.textureID);
            
            {
                SpriteVertexData vertices[4];
                
                float pos;
                pos = 0.2;
                
                v3f& pos0 = vertices[0].position;
                pos0.x = -pos; pos0.y = -pos; pos0.z = 0.0;
                v3f& pos1 = vertices[1].position;
                pos1.x = pos; pos1.y = -pos; pos1.z=0;
                v3f& pos2 = vertices[2].position;
                pos2.x = pos; pos2.y = pos; pos2.z=0;
                v3f& pos3= vertices[3].position;
                pos3.x = -pos; pos3.y = pos; pos3.z=0;
                
                v2f& uv0 = vertices[0].uv;
                uv0.x = 0.0; uv0.y = 0;
                v2f& uv1 = vertices[1].uv;
                uv1.x = 1.0; uv1.y = 0;
                v2f& uv2 = vertices[2].uv;
                uv2.x = 1.0; uv2.y = 1;
                v2f& uv3 = vertices[3].uv;
                uv3.x = 0.0; uv3.y = 1;
                
                
                glBufferSubData(GL_ARRAY_BUFFER, spriteSheet.renderer.vertexCount * sizeof(SpriteVertexData),
                                4 * sizeof(SpriteVertexData),
                                &vertices[0]);
                
            }
            
            spriteSheet.renderer.vertexCount += 4;
            ++spriteSheet.renderer.drawCount;
            
            
            for (int i = 0; i < Sprites.size(); ++i)
            {
                const Sprite& spr = Sprites[i];
                
                SpriteVertexData vertices[4] = {};
                
                v3f& pos0 = vertices[0].position;
                pos0.x = 0.2 * i; pos0.y = 0.2 * i; pos0.z = -1.0;
                v3f& pos1 = vertices[1].position;
                pos1.x = 0.2 * i + 0.1; pos1.y = 0.2 * i; pos1.z=-1;
                v3f& pos2 = vertices[2].position;
                pos2.x =  0.2 * i + 0.1; pos2.y =  0.2 * i + 0.1; pos2.z=-1;
                v3f& pos3= vertices[3].position;
                pos3.x = 0.2 * i; pos3.y = 0.2 * i + 0.1; pos3.z=-1;
                
                v2f& uv0 = vertices[0].uv;
                uv0.x = spr.bl_coord.x; uv0.y = spr.bl_coord.y;
                v2f& uv1 = vertices[1].uv;
                uv1.x = spr.ur_coord.x; uv1.y = spr.bl_coord.y;
                v2f& uv2 = vertices[2].uv;
                uv2.x =  spr.ur_coord.x; uv2.y = spr.ur_coord.y;
                v2f& uv3 = vertices[3].uv;
                uv3.x =  spr.bl_coord.x; uv3.y = spr.ur_coord.x;
                
                glBufferSubData(GL_ARRAY_BUFFER,
                                spriteSheet.renderer.vertexCount*sizeof(SpriteVertexData),
                                4*sizeof(SpriteVertexData),
                                &vertices[0]);
                
                spriteSheet.renderer.vertexCount += 4;
                ++spriteSheet.renderer.drawCount;
            }
            
            glDrawElements(GL_TRIANGLES, spriteSheet.renderer.drawCount * 6, GL_UNSIGNED_INT, 0);
            
            spriteSheet.renderer.vertexCount = 0;
            spriteSheet.renderer.drawCount = 0;
        }
        
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    CleanupRenderer(&spriteSheet.renderer);
    CleanupSpriteSheet(&spriteSheet);
    
    glfwTerminate();
    return 0;
}