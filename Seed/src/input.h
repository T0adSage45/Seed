#pragma once

#include <pch.h>
#include "core.h"

namespace Seed {

class SEED_API InputManager {
public:
    inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

    inline static bool IsMousePressed(int button) { return s_Instance->IsMousePressedImpl(button); }
    inline static float GetMouseScroll() { return s_Instance->IsMouseScroll(); };

    inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); };

protected:
    virtual bool IsKeyPressedImpl(int keycode) = 0;
    virtual bool IsMousePressedImpl(int button) = 0;
    virtual std::pair<float, float> GetMousePosImpl() = 0;
    virtual float IsMouseScroll() = 0;

private:
    static InputManager *s_Instance;
}; // namespace Seed

} // namespace Seed
