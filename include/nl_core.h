
#ifndef NL_CORE_H_
#define NL_CORE_H_

// Was considering creating 1 class that holds everything to pass around
// A WIP

#include <nl_camera.h>
#include <nl_debug.h>
#include <nl_framework.h>
#include <nl_game.h>
#include <nl_input.h>
#include <nl_key.h>
#include <nl_math.h>
#include <nl_platform.h>
#include <nl_renderer.h>
#include <nl_shader.h>
#include <nl_spritesheet.h>
#include <nl_timer.h>
#include <nl_ui.h>
#include <nl_utils.h>
#include <nl_viewport.h>

struct Core
{
    struct Platform* platform;
    struct Input* input;
    struct Framework* framework;

    struct GameData* game;

    struct UI* active_ui;
};

#endif //NL_CORE_H_
