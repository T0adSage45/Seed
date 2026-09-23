#pragma once
#include <pch.h>
#include "core.h"
#include "input.h"

namespace Seed {

class SEED_API Linux_Input : public InputManager {
protected:
    virtual bool IsKeyPressedImpl(int keycode) override;
    virtual bool IsMousePressedImpl(int button) override;
    virtual std::pair<float, float> GetMousePosImpl() override;
    virtual float IsMouseScroll() override;
};

} // namespace Seed
