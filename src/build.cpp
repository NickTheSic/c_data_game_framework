
#include <main.cpp>
#include <nl_camera.cpp>
#include <nl_collision.cpp>
#include <nl_framework.cpp>
#include <nl_gl.cpp>
#include <nl_grid.cpp>
#include <nl_input.cpp>
#include <nl_math.cpp>
#include <nl_renderer.cpp>
#include <nl_shader.cpp>
#include <nl_spritesheet.cpp>
#include <nl_timer.cpp>
#include <nl_ui.cpp>
#include <nl_viewport.cpp>

// Specific Implementations
//#include <nl_audio_openal.cpp>
#include <nl_controller_xinput.cpp>

// Game
#include <nl_game.cpp>

// Platform
#if defined(_WIN32) || defined(PLATFORM_WINDOWS)
#include <nl_platform_win32.cpp>
#elif defined(__EMSCRIPTEN__) || defined(PLATFORM_WEB)
#include <nl_platform_web.cpp>
#else
#error "Incomplete Platform or Unregistered platform layer"
#endif
