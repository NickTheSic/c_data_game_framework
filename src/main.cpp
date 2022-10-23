#include <stdio.h>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Types.h>
#include <Renderer.h>

#include <string>

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
        InitializeSpriteSheet(&spriteSheet, 256, 256);
        CompileSpriteShaderProgram(&spriteSheet.renderer);
    }
    
    int keyState = glfwGetKey(window, GLFW_KEY_E);
    
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
    
    int tileCount = 0;
    std::string tilesPath = "data/kenney/tile_00";
    
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
                {
                    std::string tilesPathFull = tilesPath;
                    if (tileCount < 10)
                    {
                        tilesPathFull.append("0");
                        tilesPathFull.append(std::to_string(tileCount));
                    }
                    else if (tileCount < 100)
                    {
                        tilesPathFull.append(std::to_string(tileCount));
                    }
                    tilesPathFull.append(".png");
                    ++tileCount;
                    
                    Sprite spr = {};
                    spr.size.x = 0.2f;
                    spr.size.y = 0.2f;
                    if (LoadSprite(&spriteSheet, &spr, tilesPathFull.c_str()))
                    {
                        spr.position = Positions[posIndex++%posCounts];
                        Sprites.push_back(spr);
                    }else{
                        fprintf(stderr, "Failed to add sprite\n");
                    }
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
            
            DisplayEntireSheet(&spriteSheet, {-0.2,-0.2,-1}, {0.4,0.4});
            
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