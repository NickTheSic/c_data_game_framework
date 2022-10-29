#include <stdio.h>
#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#include <nl_math.h>
#include <nl_renderer.h>
#include <Camera.h>
#include <nl_shader.h>

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
    glEnable(GL_DEPTH);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glViewport(0, 0, 800, 800);
    glClearColor(0.1,0.2,0.2,1.0);
    
    // Custom Init
    v3f player_pos(0.f,0.f,1.f);
    FollowCamera cam = {};
    cam.position = &player_pos;
    
    Shader shader = {};
    SpriteSheet spriteSheet = {};
    {
        InitializeRenderer(&spriteSheet.renderer, 8, sizeof(SpriteVertexData));
        InitializeSpriteSheet(&spriteSheet, 512, 512);
        
        CreateFollowViewMatrix(&cam);
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
    
    SpriteHandle sprite_handle_1 = LoadSprite(&spriteSheet, "data/blue64.png");

    InitializeSpriteAnim(&anims[1], 3, 10);
    anims[1].sprite_handles[0] = LoadSprite(&spriteSheet, "data/testanimattack-01.png");
    anims[1].sprite_handles[1] = LoadSprite(&spriteSheet, "data/testanimattack-02.png");
    anims[1].sprite_handles[2] = LoadSprite(&spriteSheet, "data/testanimattack-03.png");
    anims[1].callback = &UnAttackAnim;
    
    SpriteHandle sprite_handle_2 = LoadSprite(&spriteSheet, "data/blue64.png");
    
    double now = glfwGetTime();
    double last = now;
    
    int actionKey = glfwGetKey(window, GLFW_KEY_E);
    
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
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            player_pos.y += 1 * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            player_pos.y -= 1 * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            player_pos.x -= 1 * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            player_pos.x += 1 * deltaTime;
        }
        
        CreateFollowViewMatrix(&cam);
        SetUniform(&shader, "view", cam.view);
        
        // custom render code
        {
            BeginRender(&spriteSheet.renderer);
            glBindTexture(GL_TEXTURE_2D, spriteSheet.textureID);
            
            RenderSpriteAnimationFrame(&spriteSheet, &anims[ActiveAnim], player_pos);
            
            float offset_pos = 0.3;
            AddSpriteToRender(&spriteSheet, sprite_handle_1, v3f(-offset_pos, -offset_pos, -0.0));
            AddSpriteToRender(&spriteSheet, sprite_handle_2, v3f(offset_pos, offset_pos, 0.0));
            
            DisplayEntireSheet(&spriteSheet, {-0.1,-0.1, 1}, {0.4f,0.4f});
            
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
        return 0;
    }
    glfwMakeContextCurrent(window);
    
    if (LoadGLExtensions() == 0)
    {
        fprintf(stderr,"Failed to load opengl extensions\n");
        
        return 0;
    }
    
    return 1;
}

