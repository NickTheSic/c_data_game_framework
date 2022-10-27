#include <stdio.h>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <nl_math.h>
#include <Renderer.h>
#include <Camera.h>
#include <Shader.h>

#include <string>

int InitPlatform(GLFWwindow*& window);

int ActiveAnim = 0;
void UnAttackAnim()
{
    ActiveAnim = 0;
}

bool HandleKeyPress(GLFWwindow* window, int& state, unsigned int GlfwKey)
{
    int key = glfwGetKey(window, GlfwKey);
    if (key != state)
    {
        state = key;
        if (state == GLFW_PRESS)
        {
            return true;
        }
    }
    return false;
}

int 
main()
{
    GLFWwindow* window = 0;
    InitPlatform(window);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, 800, 800);
    glClearColor(0.1,0.2,0.2,1.0);
    
    // Custom Init
    Camera cam = {};
    
    Shader shader = {};
    SpriteSheet spriteSheet = {};
    {
        InitializeRenderer(&spriteSheet.renderer, 8, sizeof(SpriteVertexData));
        InitializeSpriteSheet(&spriteSheet, 256, 256);

        CreateViewMatrix(&cam, v3f(0.0f,0.f,1.f), v3f(0.f,0.f,0.f));
        CompileSpriteShaderProgram(&spriteSheet.renderer);
        shader.program = spriteSheet.renderer.shader_program;
        SetUniform(&shader, "view", cam.view);
    }
    
    SpriteAnimation anims[2] = {};
    InitializeSpriteAnim(&anims[0], 4, 5);
    
    anims[0].sprite_handles[0] = LoadSprite(&spriteSheet, "data/testanim-01.png");
    anims[0].sprite_handles[1] = LoadSprite(&spriteSheet, "data/testanim-02.png");
    anims[0].sprite_handles[2] = LoadSprite(&spriteSheet, "data/testanim-03.png");
    anims[0].sprite_handles[3] = LoadSprite(&spriteSheet, "data/testanim-04.png");
    
    InitializeSpriteAnim(&anims[1], 3, 10);
    anims[1].sprite_handles[0] = LoadSprite(&spriteSheet, "data/testanimattack-01.png");
    anims[1].sprite_handles[1] = LoadSprite(&spriteSheet, "data/testanimattack-02.png");
    anims[1].sprite_handles[2] = LoadSprite(&spriteSheet, "data/testanimattack-03.png");
    anims[1].callback = &UnAttackAnim;
    
    double now = glfwGetTime();
    double last = now;
    
    int actionKey = glfwGetKey(window, GLFW_KEY_E);
    int upKey = glfwGetKey(window, GLFW_KEY_W);
    int downKey = glfwGetKey(window, GLFW_KEY_S);
    int leftKey = glfwGetKey(window, GLFW_KEY_A);
    int rightKey = glfwGetKey(window, GLFW_KEY_D);

    v3f PlayerPos(0.f,0.f,0.f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        now = glfwGetTime();
        float deltaTime = (float)(now-last);
        last = now;
        
        UpdateSpriteAnim(&anims[ActiveAnim], deltaTime);
        
        if (HandleKeyPress(window, actionKey, GLFW_KEY_E))
        {
            ActiveAnim = 1;
        }
        if (HandleKeyPress(window, upKey, GLFW_KEY_W))
        {
            PlayerPos.y += 0.1;
        }
        if (HandleKeyPress(window, downKey, GLFW_KEY_S))
        {
            PlayerPos.y -= 0.1;
        }
         if (HandleKeyPress(window, leftKey, GLFW_KEY_A))
        {
            PlayerPos.x -= 0.1;
        }
        if (HandleKeyPress(window, rightKey, GLFW_KEY_D))
        {
            PlayerPos.x += 0.1;
        }
        
        // custom render code
        {
            glUseProgram(spriteSheet.renderer.shader_program);
            glBindVertexArray(spriteSheet.renderer.vao);
            glBindBuffer(GL_ARRAY_BUFFER, spriteSheet.renderer.vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spriteSheet.renderer.ebo);
            glBindTexture(GL_TEXTURE_2D, spriteSheet.textureID);
            
            DisplayEntireSheet(&spriteSheet, {-0.1,-0.1, 1}, {0.4f,0.4f});
            
            RenderSpriteAnimationFrame(&spriteSheet, &anims[ActiveAnim], PlayerPos);
            
            EndRender(&spriteSheet.renderer);
        }
        
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT);
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

