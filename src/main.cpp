#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include <GLFW/glfw3.h>

#include <nl_camera.h>
#include <nl_game.h>
#include <nl_gl.h>
#include <nl_math.h>
#include <nl_renderer.h>
#include <nl_shader.h>

int InitPlatform(GLFWwindow*& window);

bool 
HandleKeyPress(GLFWwindow* window, int& state, unsigned int GlfwKey)
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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, 800, 800);
    glClearColor(0.1,0.2,0.2,1.0);
    
    // Custom Init
    
    GameData game_data = {};
    GameInitialize(&game_data);
    
    int actionKey = glfwGetKey(window, GLFW_KEY_E);
    
    double now = glfwGetTime();
    double last = now;
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        now = glfwGetTime();
        const float deltaTime = (float)(now-last);
        last = now;
        
        GameUpdate(&game_data, deltaTime);
        
        if (HandleKeyPress(window, actionKey, GLFW_KEY_E))
        {
            game_data.active_player_anim = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            game_data.player_pos.y += 1 * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            game_data.player_pos.y -= 1 * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            game_data.player_pos.x -= 1 * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            game_data.player_pos.x += 1 * deltaTime;
        }
        
        // custom render code
        {
            GameRender(&game_data);
        }
        
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    GameCleanup(&game_data);
    
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

