#pragma once
#include <pch.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "runtime.h"
#include "window.h"

namespace Seed {

class _Sdl_Window : public Window {
public:
    _Sdl_Window(const WindowProps &props);
    virtual ~_Sdl_Window();

    void OnUpdate(Timestep delta) override;

    inline unsigned int GetWidth() const override { return seed_data.Width; };
    inline unsigned int GetHeight() const override { return seed_data.Height; };

    inline void SetEventCallback(const EventCallbackFn &callback) override {
        seed_data.EventCallback = callback;
    };

    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    inline void *GetNativeWindow() const override { return seed_data.seed_Window; };
    inline void *GetGLContext() const override { return seed_data.seed_glcontext; };

private:
    virtual void Init(const WindowProps &props);
    virtual void Shutdown();

private:
    struct WindowData {
        SDL_Window *seed_Window;
        SDL_GLContext seed_glcontext;
        SDL_Event seed_event;
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData seed_data;
};

} // namespace Seed
