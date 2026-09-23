#pragma once

#include "pch.h"
#include "core.h"
#include "layers.h"

namespace Seed {

class SEED_API LayerStack {
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer *l);
    void PopLayer(Layer *l);
    void PushOverlay(Layer *l);
    void PopOverlay(Layer *l);

    std::vector<Seed::Scope<Layer>>::reverse_iterator rbegin() { return layerstack.rbegin(); }
    std::vector<Seed::Scope<Layer>>::reverse_iterator rend() { return layerstack.rend(); }

    std::vector<Seed::Scope<Layer>>::iterator begin() { return layerstack.begin(); }
    std::vector<Seed::Scope<Layer>>::iterator end() { return layerstack.end(); }

private:
    std::vector<Seed::Scope<Layer>> layerstack;
    unsigned int layerstackInsert = 0;
};

} // namespace Seed
