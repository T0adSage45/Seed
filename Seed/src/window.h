#pragma once
#include "pch.h"
#include "core.h"
#include "events.h"
#include "runtime.h"

namespace Seed {

struct WindowProps {
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string &title = "SEED Window", unsigned int width = 700,
                unsigned int height = 500)
        : Title(title),
          Width(width),
          Height(height) {}
};

class SEED_API Window {
public:
    using EventCallbackFn = std::function<void(Event &)>;

    virtual ~Window() { Seed_Warn("window destroyed..."); };

    virtual void OnUpdate(Timestep delta) = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    virtual void Resized() = 0;

    virtual void SetEventCallback(const EventCallbackFn &callback) = 0;

    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void *GetNativeWindow() const = 0;
    virtual void *GetGLContext() const = 0;

    static Window *Create(const WindowProps &props = WindowProps());
};

} // namespace Seed
