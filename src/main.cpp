#include <stdio.h>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Types.h>
#include <Renderer.h>

#include <string>

int InitPlatform(GLFWwindow*& window);

int ActiveAnim = 0;

void UnAttackAnim()
{
    ActiveAnim = 0;
}

int 
main()
{
    GLFWwindow* window = 0;
    InitPlatform(window);
    
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
    
    SpriteAnimation anims[2] = {};
    InitializeSpriteAnim(&anims[0], 4, 5, {.2f,.2f});
    
    LoadSprite(&spriteSheet, &anims[0].sprites[0], "data/testanim-01.png");
    LoadSprite(&spriteSheet, &anims[0].sprites[1], "data/testanim-02.png");
    LoadSprite(&spriteSheet, &anims[0].sprites[2], "data/testanim-03.png");
    LoadSprite(&spriteSheet, &anims[0].sprites[3], "data/testanim-04.png");

    InitializeSpriteAnim(&anims[1], 3, 5, {.2f,.2f});
    anims[1].callback = &UnAttackAnim;
    LoadSprite(&spriteSheet, &anims[1].sprites[0], "data/testanimattack-01.png");
    LoadSprite(&spriteSheet, &anims[1].sprites[1], "data/testanimattack-02.png");
    LoadSprite(&spriteSheet, &anims[1].sprites[2], "data/testanimattack-03.png");
    
    double now = glfwGetTime();
    double last = now;
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        now = glfwGetTime();
        float deltaTime = (float)(now-last);
        last = now;
        
        UpdateSpriteAnim(&anims[ActiveAnim], deltaTime);
        
        int state = glfwGetKey(window, GLFW_KEY_E);
        if (state != keyState)
        {
            keyState = state;
            if (state == GLFW_PRESS)
            {
                ActiveAnim = 1;
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
            
            for (unsigned long long i = 0; i < Sprites.size(); ++i)
            {
                AddSpriteToRender(&spriteSheet, &Sprites[i],  Positions[posIndex%posCounts]);
            }
            
            RenderSpriteAnimationFrame(&spriteSheet, &anims[ActiveAnim], Positions[posIndex%posCounts]);
            
            EndRender(&spriteSheet.renderer);
        }
        
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    CleanupSpriteAnimation(&anims[0]);
    CleanupSpriteAnimation(&anims[1]);
    CleanupRenderer(&spriteSheet.renderer);
    CleanupSpriteSheet(&spriteSheet);
    
    glfwTerminate();
    return 0;
}



int 
InitPlatform(GLFWwindow*& window)
{
    // int glfw and other platform stuff
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
    
    return 0;
}

