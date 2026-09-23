
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <seed.h> // necessory to implent the factory pattern
#include "app.h"
#include "imgui.h"
#include "init.h"
#include "layers/layers.h"

struct Editor_Data {
    char *previous_state = nullptr;
};

class Editor : public Seed::Layer {
private:
    Editor_Data m_Gdata;
    Seed::ShaderLib m_Shaderlibrary;

public:
    ~Editor() {};

    void OnAttach() override {
        m_Shaderlibrary.Load("leaf/shader/texture.glsl");
        m_Shaderlibrary.Load("leaf/shader/flat.glsl");
        m_Shaderlibrary.Load("leaf/shader/toon.glsl");

        if (m_Gdata.previous_state != nullptr) {
            Phyll::Load(m_Gdata.previous_state);
        }
    };

    void OnEvent(Seed::Event &e) override { (void)&e; };

    void OnUpdate(Seed::Timestep delta) override {
        (void)&delta;

        Seed::RenderCmd::SetClearColor({.2f, .2f, .2f, 1.0f});
        Seed::RenderCmd::Clear();
    };

    void OnImGuiDrawCall() override {};
};

class Leaf : public Seed::Application {
public:
    Leaf() {
        Seed_Trace("Memory Allocated on the heap....");

        Editor *kuro = new Editor(); // look at me .... (raw pointer issue);
        PushLayer(kuro);
    };
    ~Leaf() { Seed_Warn("Destroyed the Allocated mem...."); };
};

Seed::Application *Seed::CreateApp() {
    Seed_Trace("application created....");
    return new Leaf();
};
