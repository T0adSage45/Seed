#include <chrono>
#include <pch.h>
#include "app.h"
#include "events.h"
#include "log.h"
#include "runtime.h"
#include "ui.h"
#include "window.h"

namespace Seed {

// singleton
Application *Application::s_instance = nullptr;

// constructor
Application::Application() {

    if (s_instance == nullptr) {
        s_instance = this;
    }

    Seed_Trace("application class triggered");
    seed_Window = std::unique_ptr<Window>(Window::Create());
    (*seed_Window).SetEventCallback(SEED_BIND_EVENT_FN(&Application::OnEvent));
    seed_Window->SetVSync(true);

    seed_DuiLayer = new DebugUi();
    seed_DuiLayer->IsActive = true;
    PushOverlay(seed_DuiLayer);
}

// destructor
Application::~Application() { Seed_Warn("application closed.."); }

// Main evet dispatcher
void Application::OnEvent(Event &e) {
    EventDispatcher dispatcher(e);

    //// winow events
    /// TODO:: window as a layer
    dispatcher.Dispatch<WindowClosedEvent>(SEED_BIND_EVENT_FN(&Application::OnWindowClosed));
    dispatcher.Dispatch<WindowResizedEvent>([](WindowResizedEvent &e) {
        (void)e;
        Seed_Info("Window resized: %d x %d", e.GetWidth(), e.GetHeight());
        return false;
    });
    dispatcher.Dispatch<WindowMovedEvent>([](WindowMovedEvent &e) {
        (void)e;
        Seed_Info("Window moved to: %d, %d", e.GetX(), e.GetY());
        return false;
    });

    // layer stack (from top to bottom)
    for (auto it = l_stacks.rbegin(); it != l_stacks.rend(); ++it) {
        if ((*it)->IsActive) {
            (*it)->OnEvent(e); // current events passed to layerr
            if (e.IsHandled())
                break;
        }
        // skip layer
    }
}

// on repeat main game loop
void Application::Run() {

    Seed_Trace("たね。。。 initiated..");
    seed_running = true;

    while (seed_running) {
        auto curr_timestamp = std::chrono::steady_clock::now();
        Timestep delta = std::chrono::duration<float>(curr_timestamp - lastframe_time).count();

        // Seed_Warn("the current delta : %f", delta);
        // Seed_Fatal("end of the prog");

        for (auto &layout : l_stacks) {
            if (layout->IsActive) {
                layout->OnUpdate(delta);
            }
        }

        seed_DuiLayer->Begin();
        for (auto &layout : l_stacks) {
            if (layout->IsActive) {
                layout->OnImGuiDrawCall();
            }
        }
        seed_DuiLayer->End();
        seed_Window->OnUpdate(delta);
        lastframe_time = curr_timestamp;
    }
}

// closing handle
bool Application::OnWindowClosed(Event &e) {
    (void)e;
    Seed_Warn("Window closed event received");
    seed_running = false;
    return true;
}

} // namespace Seed
