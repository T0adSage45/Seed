#pragma once

#include <pch.h>
#include "core.h"
#include "events.h"
#include "runtime.h"

namespace Seed {

class SEED_API Layer {
public:
    Layer(const std::string &name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep delta) { (void)&delta; };
    virtual void OnImGuiDrawCall() {};
    virtual void OnEvent(Event &e) {
        if (IsActive)
            OnEventImpl(e);
    }

    std::string layerName;
    bool IsActive = true;

protected:
    virtual void OnEventImpl(Event &e) { (void)e; }
};

} // namespace Seed
