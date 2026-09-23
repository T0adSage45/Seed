#include <pthread.h>
#include "layerstack.h"

namespace Seed {

LayerStack::LayerStack() {};

LayerStack::~LayerStack() {};

void LayerStack::PushLayer(Layer *l) {
    layerstack.emplace(layerstack.begin() + layerstackInsert, l);
    l->OnAttach();
    layerstackInsert++;
}

void LayerStack::PushOverlay(Layer *o) {
    layerstack.emplace_back(o);
    o->OnAttach();
}

void LayerStack::PopLayer(Layer *l) {
    auto it = std::find_if(layerstack.begin(), layerstack.end(),
                           [l](const Seed::Scope<Layer> &ptr) { return ptr.get() == l; });

    if (it != layerstack.end()) {
        (*it)->OnDetach();
        layerstack.erase(it);
        layerstackInsert--;
    }
}

void LayerStack::PopOverlay(Layer *o) {
    auto it = std::find_if(layerstack.begin(), layerstack.end(),
                           [o](const Seed::Scope<Layer> &ptr) { return ptr.get() == o; });
    if (it != layerstack.end()) {
        (*it)->OnDetach();
        layerstack.erase(it);
    }
}

} // namespace Seed
