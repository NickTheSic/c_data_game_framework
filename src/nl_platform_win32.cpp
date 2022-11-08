#include <nl_platform.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef near
#undef far

static LRESULT CALLBACK
WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = {};

    switch(msg)
    {
		case WM_DESTROY:
		case WM_QUIT:
		{
			PostQuitMessage(0);
		} break;

        default:
            result = DefWindowProc(window, msg, wParam, lParam);
    }

    return result;
}

NLPlatform* 
CreatePlatform(int width, int height, const char* title)
{
    DWORD dw_ex_style, dw_style;
	dw_ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dw_style = WS_OVERLAPPEDWINDOW;

    RECT window_rect = {};
    window_rect.left = (long)0;
    window_rect.right = (long)width;
    window_rect.top = (long)0;
    window_rect.bottom = (long)height;

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

    AdjustWindowRectEx(&window_rect, dw_style, 0, dw_ex_style);
	
    NLPlatform* platform = new NLPlatform;

    platform->window = CreateWindowEx(
        dw_ex_style,
		wc.lpszClassName,
		title,
		dw_style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(window_rect.right - window_rect.left),
		(window_rect.bottom - window_rect.top),
		NULL, NULL, GetModuleHandle(0),
		nullptr // User Data - Needs to be parameter
    );

    if (platform->window == nullptr)
    {
        DestroyPlatform(platform);   
        return nullptr;
    }

    platform->device = GetDC((HWND)platform->window);

    if (platform->device == nullptr)
    {
        DestroyPlatform(platform);
        return nullptr;
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
        DestroyPlatform(platform);
		return nullptr;
	}

	if (!SetPixelFormat((HDC)platform->device, PixelFormat, &pfd))
	{
        DestroyPlatform(platform);
		return nullptr;
	}

	platform->context = wglCreateContext((HDC)platform->device);
	if (platform->context == nullptr)
	{
        DestroyPlatform(platform);
		return nullptr;
	}

	if (!wglMakeCurrent((HDC)platform->device, (HGLRC)platform->context))
	{
        DestroyPlatform(platform);
		return nullptr;
	}

	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwap = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwap != nullptr)
	{
		wglSwap(0);
	}

	// Displays the graphics card used
	{
		char* rendererString;
		rendererString = (char*)glGetString(GL_RENDERER);
		if (rendererString) fprintf(stderr, TEXT("%s"), rendererString);
	}


	// calculate DPI for scaling
	{
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		
		const UINT dpi = GetDpiForWindow((HWND)platform->window);
		AdjustWindowRectExForDpi(&window_rect, dw_style, 0, dw_ex_style, dpi);
		SetWindowPos(
			(HWND)platform->window,
			0,
			window_rect.left,
			window_rect.top,
			window_rect.right - window_rect.left,
			window_rect.bottom - window_rect.top,
			0);
	}

	ShowWindow((HWND)platform->window, SW_SHOW);
	SetForegroundWindow((HWND)platform->window);
	SetFocus((HWND)platform->window);

    return platform;
}

void 
DestroyPlatform(NLPlatform* platform)
{
    if (platform)
    {
        // free window, release DC
        ReleaseDC((HWND)platform->window, (HDC)platform->device);
        delete platform;
    }
}

bool 
NLPollEvents(NLPlatform* platform)
{
	//Unused, for compatibility
	(void)(platform);

	MSG msg = {};
	//while (PeekMessage(&msg, (HWND)platform->window, 0, 0, PM_REMOVE))
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message != WM_QUIT)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			return false;
		}
	}

	return true;
}

void 
NLSwapBuffers(NLPlatform* platform)
{
	SwapBuffers((HDC)platform->device);
}
