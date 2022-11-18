
#ifndef NL_KEY_H_
#define NL_KEY_H_

#ifdef GLFW_PLATFORM_LAYER
#include <GLFW/glfw3.h>
#elif defined(_WIN32)

#elif defined(__EMSCRIPTEN__)
# error Not yet implemented
#elif
# error Incomplete Platform Keys
#endif

enum class MouseButton : char
{
    NONE = -1,
    Left = 0,
    Middle,
    Right
};

enum class KeyState : unsigned char
{
    Up = 0, 
    Down = 1,
    Repeat,
};

// Make this an unsigned char or short?
enum class Key
{
#ifdef GLFW_PLATFORM_LAYER
    W = GLFW_KEY_W,
    E = GLFW_KEY_E,
    A = GLFW_KEY_A,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,

#elif defined(_WIN32)

    A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

#elif defined(__EMSCRIPTEN__)

# error Not yet implemented

#elif

# error Incomplete Platform Keys Implementation

#endif
};

#endif //NL_KEY_H_