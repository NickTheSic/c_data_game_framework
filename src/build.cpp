
#include "main.cpp"
#include "nl_camera.cpp"
#include "nl_game.cpp"
#include "nl_gl.cpp"
#include "nl_input.cpp"
#include "nl_renderer.cpp"
#include "nl_shader.cpp"
#include "nl_timer.cpp"

// Platform

#ifdef GLFW_PLATFORM_LAYER
#include "nl_platform_glfw.cpp"
#elif defined(_WIN32)
#include "nl_platform_win32.cpp"
#elif __EMSCRIPTEN__
#include "nl_platform_web.cpp"
#else
#error "Incomplete Platform"
#endif
