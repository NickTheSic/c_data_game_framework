#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include <nl_camera.h>
#include <nl_game.h>
#include <nl_gl.h>
#include <nl_grid.h>
#include <nl_input.h>
#include <nl_math.h>
#include <nl_platform.h>
#include <nl_renderer.h>
#include <nl_shader.h>
#include <nl_timer.h>
#include <nl_ui.h>

#ifdef VS2019_PROJECT
#include <crtdbg.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

void 
run(void* data)
{
    Platform* platform = static_cast<Platform*>(data);
    GameData* game_data = static_cast<GameData*>(platform->game_data);

    const float delta_time = GetTime();

    #ifdef DEBUG
    // Incomplete but I want to add a delta time modifier
    // delta_time *= time_modifier;
    #endif

    ResetScreenBuffer();
       
    GameUpdate(platform, game_data, delta_time);
    UpdateUI(&platform->ui, platform);
    
    // Might want to let the sprites render themselves if they are active,
    // Loaded sprites are different than draw sprites, drawn sprites re use loaded sprites
    SpriteSheetBeginRender(&platform->fw.sprite_sheet);
    {
        GameRender(platform, game_data);
    }
    SpriteSheetEndRender(&platform->fw.sprite_sheet);
    
    // UI Rendering, Calling begin and end so that I draw on the correct camera
    SpriteSheetBeginRender(&platform->fw.sprite_sheet);
    {
        RenderUI(&platform->ui, &platform->fw);

        // FPS Counter
        {
            static float timed_loop;
            static int fps_count;
            static int frames;
            
            timed_loop += delta_time;
            if (timed_loop > 1.0f)
            {
                fps_count = (float)frames / timed_loop;
                timed_loop -= 1.0f;
                frames = 0;
            }
            ++frames;

            std::string fps = "fps " + std::to_string(fps_count);
            HandleText(&platform->ui, fps.c_str(), {0.f, 720.f}, {16.f,16.f});
        }

        if (HandleButton(&platform->ui, {0.f, 0.f}, {32.f, 32.f}, "test",
            {(float)platform->input.mouse_pos.x, (float)platform->input.mouse_pos.y}, 
            platform->input.mouse_button[(int)MouseButton::Left] == ButtonState::Down))
        {
            ReloadTileSet(platform, (Tile*)temp_GetTilePtr(game_data), 0);
            LOG("Successful test Button Press");
            glClearColor(0.8,0.1,0.1,1);
        }
    
        if (HandleButton(&platform->ui, {0.f, 48.f}, {32.f, 32.f}, "funky",
            {(float)platform->input.mouse_pos.x, (float)platform->input.mouse_pos.y}, 
            platform->input.mouse_button[(int)MouseButton::Left] == ButtonState::Down))
        {
            ReloadTileSet(platform, (Tile*)temp_GetTilePtr(game_data), 1);
            LOG("Successful funky Button Press");
            glClearColor(0.4,0.9,0.1,1);
        }
    
        if (HandleButton(&platform->ui, {0.f, 96.f}, {32.f, 32.f}, "yes?man",
            {(float)platform->input.mouse_pos.x, (float)platform->input.mouse_pos.y}, 
            platform->input.mouse_button[(int)MouseButton::Left] == ButtonState::Down))
        {
            LOG("Successful yes?man Button Press");
        }
    
        HandleText(&platform->ui, "hello world!", {0.f, 250.f}, {16.f,16.f});
        // Can refactor to use a char[] instead of string.  I know that it would be 'x ' then up to 4 numbers then ' y ' and 4 numbers
        std::string mouse_pos_str = "x " + std::to_string(platform->input.mouse_pos.x) + " y " + std::to_string(platform->input.mouse_pos.y);
    
        HandleText(&platform->ui, mouse_pos_str.c_str(), 
            {(float)platform->input.mouse_pos.x, (float)platform->input.mouse_pos.y}, {16.f,16.f});
            
        EndUIRender(&platform->ui, &platform->fw);
    }
    SpriteSheetEndRender(&platform->fw.sprite_sheet);
    
    NLSwapBuffers(platform);
}

int 
main()
{
    #ifdef VS2019_PROJECT
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
    
    Platform* platform = CreatePlatform(1280, 800, "TEST");
    
    if (platform == 0) return 1;

    // Might want to replace this with a Renderer init
    // That way I could use a GL renderer or Vulkan or other in the future
    // I could also put this inside the CreatePlatform call or make a global init call
    if (!LoadGLExtensions()) return 2;

    // Could make an init for the renderer
    EnableBlend(true);
    EnableCullFace(true);
    CullFace(false);
    SetClearColor(0.1,0.2,0.4,1.0);

    GameData *game_data = GameInitialize(platform);

    if (game_data == 0)
    {
        return -42069;
    }

    InitUI(&platform->ui, platform, 100);

#ifndef __EMSCRIPTEN__
    while (NLPollEvents(platform))
    {
        run((void*)platform);
    };
#else
    emscripten_set_main_loop_arg(run, platform, 0, 1);
#endif
    
    CleanupUI(&platform->ui);
    GameCleanup(game_data);
    DestroyPlatform(platform);

    #ifdef VS2019_PROJECT
    _CrtDumpMemoryLeaks();
    #endif

    return 0;
}
