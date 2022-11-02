#include <nl_platform.h>
#include <Windows.h>

static LRESULT CALLBACK
WindowProc(HWND Window, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = {};

	switch (Msg)
	{
	    default:
	    {
	    	Result = DefWindowProc(Window, Msg, wParam, lParam);
	    } break;
	}

	return Result;
}

nlPlatform* 
NLCreatePlatform(int width, int height, const char* title)
{
    DWORD dwExStyle;
	DWORD dwStyle;

	RECT WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;

    WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpszClassName = TEXT("My_Window_Class");
	wc.cbClsExtra = 0;                              // No extra window data.
	wc.cbWndExtra = 0;                              // No extra window data.
	wc.hIcon = LoadIcon(0, IDI_WINLOGO);            // Load the default icon.
	wc.hCursor = LoadCursor(0, IDC_ARROW);          // Load the arrow pointer.
	wc.hbrBackground = nullptr;                     // No background required for GL.
	wc.lpszMenuName = nullptr;                      // We don't want a menu.

    if (!RegisterClassEx(&wc))
	{
		return 0;
	}

    nlPlatform *platform = (nlPlatform*)malloc(sizeof(nlPlatform));

    platform->window_width = width;
    platform->window_height = height;

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&WindowRect, dwStyle, 0, dwExStyle);

	platform->window = CreateWindowEx
	(
		dwExStyle,
		wc.lpszClassName,
		title,
		dwStyle,
		0, 0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL, NULL, GetModuleHandle(0),
		nullptr
	);

	if (platform->window == 0)
	{
        NLFreePlatform(platform);
		return 0;
	}

    platform->device = GetDC((HWND)platform->window);
    
    if (platform->device == 0)
    {
        NLFreePlatform(platform);
        return 0;
    }

	//@todo: Learn more about zbits and stencil bits
	BYTE colorbits = 32;
	BYTE zbits = 31;
	BYTE stencilbits = 1;
	PIXELFORMATDESCRIPTOR pfd =  // pfd tells Windows how we want things to be.
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this pixel format descriptor.
		1,                              // Version number.
		PFD_DRAW_TO_WINDOW |            // Format must support window.
		PFD_SUPPORT_OPENGL |            // Format must support opengl.
		PFD_DOUBLEBUFFER,               // Must support double buffering.
		PFD_TYPE_RGBA,                  // Request an rgba format.
		(BYTE)colorbits,                // Select our color depth.
		0, 0, 0, 0, 0, 0,               // Color bits ignored.
		0,                              // No alpha buffer.
		0,                              // Shift bit ignored.
		0,                              // No accumulation buffer.
		0, 0, 0, 0,                     // Accumulation bits ignored.
		(BYTE)zbits,                    // Bits for z-buffer (depth buffer).
		(BYTE)stencilbits,              // Stencil bits.
		0,                              // No auxiliary buffer.
		PFD_MAIN_PLANE,                 // Main drawing layer.
		0,                              // Reserved.
		0, 0, 0                         // Layer masks ignored.
	};

	unsigned int PixelFormat = ChoosePixelFormat((HDC)platform->device, &pfd);

	if (!PixelFormat)
	{
        NLFreePlatform(platform);
		return 0;
	}

	if (!SetPixelFormat((HDC)platform->device, PixelFormat, &pfd))
	{
        NLFreePlatform(platform);
		return 0;
	}

	platform->context = wglCreateContext((HDC)platform->device);
	if (platform->context == 0)
	{
        NLFreePlatform(platform);
		return 0;
	}

	if (!wglMakeCurrent((HDC)platform->device, (HGLRC)platform->context))
	{
        NLFreePlatform(platform);
		return 0;
	}

	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwap = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwap != nullptr)
	{
		wglSwap(0);
	}

	// Displays the graphics card used
	{
		//char* rendererString;
		//rendererString = (char*)glGetString(GL_RENDERER);
		//if (rendererString) dbglog(rendererString);
	}

	ShowWindow((HWND)platform->window, SW_SHOW);
	SetForegroundWindow((HWND)platform->window);
	SetFocus((HWND)platform->window);

    return platform;
}

void 
NLFreePlatform(nlPlatform* platform)
{
    free(platform);
}

void 
NLSwapBuffers(nlPlatform* platform)
{
    SwapBuffers((HDC)platform->device);
}

bool 
NLPollEvents(nlPlatform* platform)
{
    MSG message = {};
    while (PeekMessage(&message, (HWND)platform->window, 0, 0, PM_REMOVE))
	{
		if (message.message != WM_QUIT)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			return false;
		}
	}
	return true;
}
