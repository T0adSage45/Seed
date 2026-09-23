#include "linux_window.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "renderer/opengl.h"
#include "GL/glew.h"
#include "core.h"
#include "utility/key.h"
#include "events.h"
#include "imgui_impl_sdl3.h"
#include "runtime.h"

namespace Seed {

static bool seed_SDL_Initialized = false;

Window *Window::Create(const WindowProps &props) {
    Seed_Trace("abs window class init");
    return new _Sdl_Window(props);
}

_Sdl_Window::_Sdl_Window(const WindowProps &props) { Init(props); }

_Sdl_Window::~_Sdl_Window() { Shutdown(); }

void _Sdl_Window::Init(const WindowProps &props) {
    seed_data.Title = props.Title;
    seed_data.Width = props.Width;
    seed_data.Height = props.Height;

    if (!seed_SDL_Initialized) {
        int success = SDL_Init(SDL_INIT_VIDEO);
        SEED_CORE_ASSERT(success, "Could not initialize SDL");
        Seed_Info("SDL initialized....");
        seed_SDL_Initialized = true;
    }

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    seed_data.seed_Window =
        SDL_CreateWindow(seed_data.Title.c_str(), (int)props.Width, (int)props.Height,
                         SDL_WINDOW_RESIZABLE | SDL_WINDOW_MINIMIZED | SDL_WINDOW_OPENGL);
    if (!seed_data.seed_Window) {
        Seed_Error("seed window not created");
    }

    GLcontext rcxt(seed_data.seed_Window);
    rcxt.Init();
    seed_data.seed_glcontext = rcxt.GetGlContext();

    SetVSync(true);
}

void _Sdl_Window::Shutdown() {
    SDL_DestroyWindow(seed_data.seed_Window);
    SDL_GL_DestroyContext(seed_data.seed_glcontext);
}

void _Sdl_Window::OnUpdate(Timestep delta) {
    (void)&delta;
    while (SDL_PollEvent(&seed_data.seed_event)) {

        switch (seed_data.seed_event.type) {

        case SDL_EVENT_QUIT: {
            WindowClosedEvent e;
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            KeyPressedEvent e(SDLKey_To_Seed(seed_data.seed_event.key.scancode),
                              seed_data.seed_event.key.repeat);
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_KEY_UP: {
            KeyReleasedEvent e(SDLKey_To_Seed(seed_data.seed_event.key.scancode));
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_TEXT_INPUT: {
            KeyTypedEvent e(seed_data.seed_event.text.text[0]);
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            MouseButtonPressedEvent e(seed_data.seed_event.button.x, seed_data.seed_event.button.y,
                                      seed_data.seed_event.button.button);
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            MouseButtonReleasedEvent e(seed_data.seed_event.button.x, seed_data.seed_event.button.y,
                                       seed_data.seed_event.button.button);
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            MouseMovedEvent e(seed_data.seed_event.motion.x, seed_data.seed_event.motion.y,
                              seed_data.seed_event.motion.xrel, seed_data.seed_event.motion.yrel);
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            MouseScrolledEvent e(seed_data.seed_event.wheel.x, seed_data.seed_event.wheel.y,
                                 seed_data.seed_event.wheel.integer_x,
                                 seed_data.seed_event.wheel.integer_y);
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            seed_data.Width = seed_data.seed_event.window.data1;
            seed_data.Height = seed_data.seed_event.window.data2;
            WindowResizedEvent e(seed_data.Width, seed_data.Height);
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_WINDOW_MOVED: {
            WindowMovedEvent e(seed_data.seed_event.window.data1,
                               seed_data.seed_event.window.data2);
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_WINDOW_FOCUS_GAINED: {
            WindowFocusEvent e;
            seed_data.EventCallback(e);
            break;
        }
        case SDL_EVENT_WINDOW_FOCUS_LOST: {
            WindowLostFocusEvent e;
            seed_data.EventCallback(e);
            break;
        }
        }
    }

    SDL_GL_SwapWindow(seed_data.seed_Window);
}

void _Sdl_Window::Resized() { glViewport(0, 0, seed_data.Width, seed_data.Height); };

void _Sdl_Window::SetVSync(bool enabled) {
    if (enabled) {
        SDL_GL_SetSwapInterval(1);
    } else {
        SDL_GL_SetSwapInterval(0);
    }
    seed_data.VSync = enabled;
}

bool _Sdl_Window::IsVSync() const { return seed_data.VSync; }

} // namespace Seed
