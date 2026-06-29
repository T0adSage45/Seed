#pragma once
#include <glm/ext/vector_float3.hpp>
#include <pch.h>
#include "core.h"
#include "events.h"
#include "layerstack.h"
#include "runtime.h"
#include "ui.h"
#include "window.h"
#include <chrono>
#include <memory>

namespace Seed {
class SEED_API Application {
public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Event &e);

    inline static Application &Get() { return *s_instance; }

    inline Window &GetWindow() { return *seed_Window; }

    inline void PushLayer(Layer *l) { l_stacks.PushLayer(l); };
    inline void PushOverlay(Layer *o) { l_stacks.PushOverlay(o); };

private:
    bool OnWindowClosed(Event &e);

    std::unique_ptr<Window> seed_Window;
    DebugUi *seed_DuiLayer;
    bool seed_running = false;

    LayerStack l_stacks;
    std::chrono::time_point<std::chrono::steady_clock> lastframe_time;

private:
    static Application *s_instance;
};

SEED_API Application *CreateApp(); // to defined in client

}; // namespace Seed
