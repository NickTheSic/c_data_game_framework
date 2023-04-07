
#include <nl_audio.h>
#include <nl_camera.h>
#include <nl_collision.h>
#include <nl_controller.h>
#include <nl_debug.h>
//#include <nl_fileio.h>
#include <nl_framework.h>
#include <nl_game.h>
#include <nl_gl.h>
#include <nl_grid.h>
#include <nl_input.h>
#include <nl_key.h>
#include <nl_math.h>
#include <nl_platform.h>
#include <nl_random.h>
#include <nl_renderer.h>
#include <nl_shader.h>
#include <nl_spritesheet.h>
#include <nl_timer.h>
#include <nl_ui.h>
#include <nl_utils.h>
#include <nl_viewport.h>

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

// Specific Implementations?
// Could be added on a platform basis
#include <nl_audio_openal.cpp>

// Game
#include <nl_game.cpp>

// Platform
#if defined(_WIN32) || defined(PLATFORM_WINDOWS)
#include <nl_controller_xinput.cpp>
#include <nl_platform_win32.cpp>

#elif defined(__EMSCRIPTEN__) || defined(PLATFORM_WEB)
#include <nl_platform_web.cpp>
#include <nl_controller_null.cpp>

#else
#error "Incomplete Platform or Unregistered platform layer"
#endif
