#include <stdio.h>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Types.h>
#include <Renderer.h>

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
        "data/blue64.png",
        "data/white256x128.png",
        "data/red256.png",
        "data/blue64.png",
        "data/green128x256.png",
    };
    
    std::vector<Sprite> Sprites;
    
    int posIndex=0;
    int posCounts = 15;
    v3f Positions[] =
    {
        {-1.0f, -1.0f, 0.0f},
        {-0.6f, -1.0f, 0.0f},
        {-0.2f, -1.0f, 0.0f},
        { 0.2f, -1.0f, 0.0f},
        { 0.6f, -1.0f, 0.0f},
        {-1.0f, -0.6f, 0.0f},
        {-0.6f, -0.6f, 0.0f},
        {-0.2f, -0.6f, 0.0f},
        { 0.2f, -0.6f, 0.0f},
        { 0.6f, -0.6f, 0.0f},
        {-1.0f, -0.2f, 0.0f},
        {-0.6f, -0.2f, 0.0f},
        {-0.2f, -0.2f, 0.0f},
        { 0.2f, -0.2f, 0.0f},
        { 0.6f, -0.2f, 0.0f},
    };
    
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
                    spr.size.x = 0.2f;
                    spr.size.y = 0.2f;
                    if (LoadSprite(&spriteSheet, &spr, files[index]))
                    {
                        spr.position = Positions[posIndex++%posCounts];
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
                
                glBufferSubData(GL_ARRAY_BUFFER, spriteSheet.renderer.vertexCount * sizeof(SpriteVertexData),4 * sizeof(SpriteVertexData),&vertices[0]);
                
                spriteSheet.renderer.vertexCount += 4;
                ++spriteSheet.renderer.drawCount;
            }
            
            for (int i = 0; i < Sprites.size(); ++i)
            {
                AddSpriteToRender(&spriteSheet, &Sprites[i]);
            }
            
            EndRender(&spriteSheet.renderer);
        }
        
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    CleanupRenderer(&spriteSheet.renderer);
    CleanupSpriteSheet(&spriteSheet);
    
    glfwTerminate();
    return 0;
}