#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <seed.h> // necessory to implent the factory pattern
#include "app.h"
#include "buffers/buffer.h"
#include "imgui.h"

class Shiro : public Seed::Layer {
private:
    Seed::ShaderLib m_Shaderlibrary;
    Seed::OrhtoCamCtrl m_CameraCtrl;

private:
    Seed::Ref<Seed::Texture> m_textr;
    Seed::Ref<Seed::FrameBuffers> m_fbuff;

public:
    Shiro()
        : m_CameraCtrl(1200.0f / 720.0f, true) {};
    ~Shiro() {};

    void OnAttach() override {
        m_Shaderlibrary.Load("leaf/shader/texture.glsl");
        m_textr.reset(Seed::Texture::Create("leaf/texture/fox.png"));

        // Seed::___Seed_FrameBuff_Specs__ fbSpfss;
        // fbSpfss.width = 1260;
        // fbSpfss.height = 720;
        // fbSpfss.SwapChain_Target = false;
        // m_fbuff.reset(Seed::FrameBuffers::Create(fbSpfss));

        Seed::Renderer2D::Init();
    };

    void OnEvent(Seed::Event &e) override { m_CameraCtrl.OnEvent(e); };

    void OnUpdate(Seed::Timestep delta) override {

        // m_fbuff->Revalidate();
        // m_fbuff->Bind();

        m_CameraCtrl.OnUpdate(delta);

        Seed::RenderCmd::SetClearColor({.2f, .2f, .2f, 1.0f});
        Seed::RenderCmd::Clear();

        // batch before this
        Seed::Renderer2D::OpenScene(
            Seed::Scene{m_CameraCtrl.GetCamera()}); // look at me .... (make this took scene data...)

        Seed::Renderer2D::DrawQuad(m_Shaderlibrary.Get("texture"), m_textr, {0, 0}, {13, 18}, {-.3, 0, 0}, {3, 3, 0});

        Seed::Renderer2D::CloseScene();

        // m_fbuff->UnBind();
    };

    void OnImGuiDrawCall() override {
        // ImGui::Begin("viewport");
        // ImGui::Image(m_fbuff->GetClrAttachment(), {370, 200});
        // ImGui::End();
    };
};

class Leaf : public Seed::Application {
public:
    Leaf() {
        Seed_Trace("Memory Allocated on the heap....");

        Shiro *kuro = new Shiro(); // look at me .... (raw pointer issue);
        PushLayer(kuro);
    };
    ~Leaf() { Seed_Warn("Destroyed the Allocated mem...."); };
};

Seed::Application *Seed::CreateApp() {
    Seed_Trace("application created....");
    return new Leaf();
};
