#include "ui.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "app.h"
#include "events.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "layers/layers.h"
#include "runtime.h"

struct SDL_Window; // Forward declaration for SDL3 opaque type

namespace Seed {

Seed::DebugUi::DebugUi()
    : Layer("debugUi layer") {};

Seed::DebugUi::~DebugUi() {};

void Seed::DebugUi::OnAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
    io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform

    Application &app = Application::Get();
    ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window *>(app.GetWindow().GetNativeWindow()),
                                 app.GetWindow().GetGLContext());

    ImGui_ImplOpenGL3_Init("#version 130");
};

void Seed::DebugUi::OnUpdate(Timestep delta) {
    (void)&delta;
    SDL_Event e;
    SDL_PollEvent(&e);
    ImGui_ImplSDL3_ProcessEvent(&e);
    // not good implementation;
};

void Seed::DebugUi::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
};

void Seed::DebugUi::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    // ImGui::DockSpaceOverViewport(0, ImGui::GetWindowViewport());
};

void Seed::DebugUi::End() {
    ImGuiIO &io = ImGui::GetIO();
    Application &app = Application::Get();
    io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
};

void Seed::DebugUi::OnImGuiDrawCall() {
    // ImGui::ShowDemoWindow(&show);
};

} // namespace Seed
