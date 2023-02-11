#include <nl_platform.h>

#include <emscripten.h>
#include <emscripten/html5.h>

static Platform* g_platform = nullptr;
Platform* GetGlobalPlatform()
{
	return g_platform;
}
void SetGlobalPlatform(Platform* platform)
{
	g_platform = platform;
}

static EM_BOOL 
keyboard_callback(int eventType, const EmscriptenKeyboardEvent* e, void* platform)
{
	Platform* _platform = (Platform*)platform;

    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN)
	{
		UpdateKeyState(&_platform->input, (Key)emscripten_compute_dom_pk_code(e->code), ButtonState::Down);
	}

	if (eventType == EMSCRIPTEN_EVENT_KEYUP)
	{
		UpdateKeyState(&_platform->input, (Key)emscripten_compute_dom_pk_code(e->code), ButtonState::Up);
	}

	return EM_TRUE;
}

static EM_BOOL
mouse_callback(int event_type, const EmscriptenMouseEvent* e, void* platform)
{
	Platform* _platform = (Platform*)platform; 

    //Mouse button press
	if (e->button == 0) // left click
	{
		if (event_type == EMSCRIPTEN_EVENT_MOUSEDOWN)
		{
			UpdateMouseState(&_platform->input, MouseButton::Left, ButtonState::Down);
		}
		else if (event_type == EMSCRIPTEN_EVENT_MOUSEUP)
		{
			UpdateMouseState(&_platform->input, MouseButton::Left, ButtonState::Up);
		}
	}

	if (e->button == 2) // right click
	{
		if (event_type == EMSCRIPTEN_EVENT_MOUSEDOWN)
		{
			UpdateMouseState(&_platform->input, MouseButton::Right, ButtonState::Down);
		}
		else if (event_type == EMSCRIPTEN_EVENT_MOUSEUP)
		{
			UpdateMouseState(&_platform->input, MouseButton::Right, ButtonState::Up);
		}
	}

	return EM_FALSE;
}

static EM_BOOL
mouse_move_callback(int event_type, const EmscriptenMouseEvent* e, void* platform)
{
	UNUSED(event_type);
	Platform* _platform = (Platform*)platform;
	UpdateMousePosition(&_platform->input, e->targetX, _platform->viewport.screen_size.y - e->targetY);

	return EM_FALSE;
}

Platform* CreatePlatform(int width, int height, const char* title)
{
    EmscriptenWebGLContextAttributes attr = {};
    emscripten_webgl_init_context_attributes(&attr);

    Platform* platform = new Platform();

	emscripten_set_window_title(title);

    emscripten_set_canvas_element_size("#canvas", width, height);

	//set up callbacks here
	emscripten_set_keydown_callback("#canvas", platform, 1, keyboard_callback);
	emscripten_set_keyup_callback("#canvas", platform, 1, keyboard_callback);

	//emscripten_set_wheel_callback("#canvas", this, 1, wheel_callback);
	emscripten_set_mousedown_callback("#canvas", platform, 1, mouse_callback);
	emscripten_set_mouseup_callback("#canvas", platform, 1, mouse_callback);
	emscripten_set_mousemove_callback("#canvas", platform, 1, mouse_move_callback);

	EGLint const attribute_list[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
	EGLint const context_config[] = { EGL_CONTEXT_CLIENT_VERSION , 2, EGL_NONE };
	EGLint num_config = {};
		
	EGLConfig  config = {};

	platform->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(platform->display, nullptr, nullptr);
	eglChooseConfig(platform->display, attribute_list, &config, 1, &num_config);
		
	/* create an EGL rendering context */
	platform->surface = eglCreateWindowSurface(platform->display, config, NULL, nullptr);
	platform->context = eglCreateContext(platform->display, config, EGL_NO_CONTEXT, context_config);
		
	eglMakeCurrent(platform->display, platform->surface, platform->surface, platform->context);
	
	UpdateScreenSize(&platform->viewport, width, height);
	glViewport(0,0, width, height);

    return platform;
}


void DestroyPlatform(Platform* platform)
{
    // I don't actually think the web needs to clean up or we did something wrong?
    UNUSED(platform);
}

bool NLPollEvents(Platform* platform)
{
	// Emscripten Polls events for me
    UNUSED(platform);
    return true;
}

void NLSwapBuffers(Platform* platform)
{
	//UNUSED(platform);
    eglSwapBuffers(platform->display, platform->surface);
}

void NLSetWindowShouldClose(Platform* platform)
{
    UNUSED(platform);
}
