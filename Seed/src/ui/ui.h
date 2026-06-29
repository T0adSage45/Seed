
#pragma once

#include "core.h"
#include "imgui.h"
#include "layers.h"
#include "runtime.h"

namespace Seed {

ImGuiKey SeedKey_To_ImGui(int seed_keycode);

class SEED_API DebugUi : public Layer {
public:
    DebugUi();
    ~DebugUi();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep delta) override;
    virtual void OnImGuiDrawCall() override;

    static void Begin();
    static void End();
};
} // namespace Seed
