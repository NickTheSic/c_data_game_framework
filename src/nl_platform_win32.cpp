#include <nl_platform.h>
#include <nl_input.h>
#include <nl_key.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>
#undef near
#undef far

static Platform* g_platform = nullptr;

Platform* GetGlobalPlatform()
{
	return g_platform;
}

void SetGlobalPlatform(Platform* platform)
{
	g_platform = platform;
}

static LRESULT CALLBACK
WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = {};

	Platform* platform = (Platform*)(GetWindowLongPtr(window, GWLP_USERDATA));
	//NOTE Only windows that have the CS_DBLCLKS style can receive WM_(L/R/M)BUTTONDBLCLK messages

    switch(msg)
    {
		case WM_NCCREATE:
		{
			result = DefWindowProc(window, msg, wParam, lParam);
		} break;
		case WM_CREATE:
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			platform = (Platform*)pcs->lpCreateParams;
			SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)(platform));
			result = DefWindowProc(window, msg, wParam, lParam);
		} break;

		case WM_DESTROY:
		case WM_QUIT:
		{
			PostQuitMessage(0);
		} break;

		case WM_SIZE:
		{	
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			
			glViewport(0,0, width, height);
			
			UpdateScreenSize(&platform->viewport, width, height);
		} break;
		
		case WM_KEYUP:
		case WM_KEYDOWN:
		{
			bool is_down  = (lParam & (1 << 31)) == 0;
			bool was_down = (lParam & (1 << 30)) != 0;

			if (was_down != is_down)
			{
				UpdateKeyState(&platform->input, static_cast<Key>(wParam), static_cast<ButtonState>(is_down));
			}

			if (is_down && wParam == VK_ESCAPE)
			{
				NLSetWindowShouldClose(0);
			}
		} break;

		case WM_MOUSEMOVE:
		{
			int mouse_x_pos = GET_X_LPARAM(lParam);
			int mouse_y_pos = GET_Y_LPARAM(lParam);

			// Handling the raw position and letting the viewport/camera calculate the proper position in game
			UpdateMousePosition(&platform->input, mouse_x_pos, platform->viewport.screen_size.y - mouse_y_pos);
		} break;

		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		{
			UpdateMouseState(&platform->input, MouseButton::Left, (wParam&0x0001) ? ButtonState::Up : ButtonState::Down);
		} break;

		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		{
			UpdateMouseState(&platform->input, MouseButton::Middle, (wParam&0x0010) ? ButtonState::Up : ButtonState::Down);
		} break;

		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		{
			UpdateMouseState(&platform->input, MouseButton::Right, (wParam&0x0002) ? ButtonState::Up : ButtonState::Down);
		} break;

        default:
		{
            result = DefWindowProc(window, msg, wParam, lParam);
		} break;
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
		platform
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
		if (rendererString) fprintf(stderr, TEXT("%s\n"), rendererString);
	}

	// calculate DPI for scaling
	{
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		
		const UINT dpi = GetDpiForWindow((HWND)platform->window);
		AdjustWindowRectExForDpi(&window_rect, dw_style, 0, dw_ex_style, dpi);
		SetWindowPos(
			(HWND)platform->window,
			0,
			window_rect.left,
			window_rect.top,
			window_rect.right - window_rect.left,
			window_rect.bottom - window_rect.top,
			SWP_NOMOVE);
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
		// Assume that this is valid even though we let the game decide the resources
		CleanupFramework(&platform->fw);

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

void 
NLSetWindowShouldClose(NLPlatform* platform)
{
	(void)(platform);
	PostQuitMessage(0);
}
