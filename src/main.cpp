#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include <GLFW/glfw3.h>

#include <nl_camera.h>
#include <nl_game.h>
#include <nl_gl.h>
#include <nl_input.h>
#include <nl_math.h>
//#include <nl_platform.h>
#include <nl_renderer.h>
#include <nl_shader.h>

int InitPlatform(GLFWwindow*& window);

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/html5.h>

void em_run(void* data)
{
    GameData* game_data = (GameData*)data;

    glfwPollEvents();
        
    now = glfwGetTime();
    const float deltaTime = (float)(now-last);
    last = now;
    
    GameUpdate(&game_data, deltaTime);
    GameRender(&game_data);
    
    //glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif

int 
main()
{
    GLFWwindow* window = 0;
    InitPlatform(window);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, 800, 800);
    glClearColor(0.1,0.2,0.2,1.0);
    
    // Custom Init
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scan, int action, int mode) {
        (void)window; (void)scan; (void)mode;
        HandleAction(action, key);
    });

    GameData game_data = {};
    GameInitialize(&game_data);
    
    double now = glfwGetTime();
    double last = now;
    
    #ifndef __EMSCRIPTEN__
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        now = glfwGetTime();
        const float deltaTime = (float)(now-last);
        last = now;
        
        GameUpdate(&game_data, deltaTime);

        // custom render code
        {
            GameRender(&game_data);
        }
        
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    #else
    emscripten_set_main_loop_arg(em_run, &game_data, 0, 1);
    #endif
    
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

