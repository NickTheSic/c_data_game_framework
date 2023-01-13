
#ifndef NL_KEY_H_
#define NL_KEY_H_

#if defined(_WIN32) || defined(PLATFORM_WINDOWS)
#else
# error Incomplete Platform Keys
#endif

enum class MouseButton : unsigned char
{
    Left = 0,
    Middle,
    Right
};

enum class ButtonState : unsigned char
{
    Up = 0, 
    Down = 1,
    Repeat,
};

// Make this an unsigned char or short?
enum class Key
{
#if defined(_WIN32)

    A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

#elif defined(__EMSCRIPTEN__)

# error Not yet implemented

#elif

# error Incomplete Platform Keys Implementation

#endif
};

#endif //NL_KEY_H_