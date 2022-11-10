#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include <nl_camera.h>
#include <nl_game.h>
#include <nl_gl.h>
#include <nl_input.h>
#include <nl_math.h>
#include <nl_platform.h>
#include <nl_renderer.h>
#include <nl_shader.h>
#include <nl_timer.h>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/html5.h>

void em_run(void* data)
{
    GameData* game_data = (GameData*)data;

    const float delta_time = GetTime();

    GameUpdate(game_data, delta_time);
    GameRender(game_data);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif

int 
main()
{
    NLPlatform* platform = CreatePlatform(800, 800, "TEST");
    if (platform == 0) return 1;
    if (!LoadGLExtensions()) return 2;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, 800, 800);
    glClearColor(0.1,0.2,0.2,1.0);

    InitTime();

    GameData* game_data = 0;
    GameInitialize(game_data);

    if (game_data == 0)
    {
        return -42069;
    }
    
    #ifndef __EMSCRIPTEN__

    while (NLPollEvents(platform))
    {
        const float delta_time = GetTime();
        
        GameUpdate(game_data, delta_time);

        // custom render code
        {
            GameRender(game_data);
        }
        
        NLSwapBuffers(platform);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    #else
    emscripten_set_main_loop_arg(em_run, game_data, 0, 1);
    #endif
    
    GameCleanup(game_data);
    DestroyPlatform(platform);
    return 0;
}
