#include "buffers/buffer.h"
#include "camera/camera.h"
#include "core.h"
#include "imgui.h"
#include "layers/layers.h"
#include "render.h"
#include "texture.h"
#include <glm/gtc/type_ptr.hpp>

class Kuro : public Seed::Layer {
private:
    Seed::ShaderLib m_Shaderlibrary;
    Seed::OrhtoCamCtrl m_CameraCtrl;

private:
    Seed::Ref<Seed::Texture> m_textr, m_textr1, m_textr2;
    glm::vec2 TextCoord = {0, 0};
    glm::vec2 TextSize = {20, 20};

public:
    Kuro()
        : m_CameraCtrl(1200.0f / 720.0f, false) {};
    ~Kuro() {};

    void OnAttach() override {
        m_Shaderlibrary.Load("leaf/shader/texture.glsl");
        m_Shaderlibrary.Load("leaf/shader/flat.glsl");
        m_Shaderlibrary.Load("leaf/shader/toon.glsl");
        m_textr.reset(Seed::Texture::Create("leaf/texture/TX Props.png"));
        m_textr1.reset(Seed::Texture::Create("leaf/texture/TX Plant.png"));
        m_textr2.reset(Seed::Texture::Create("leaf/texture/fox.png"));
    };

    void OnEvent(Seed::Event &e) override { m_CameraCtrl.OnEvent(e); };

    void OnUpdate(Seed::Timestep delta) override {
        m_CameraCtrl.OnUpdate(delta);

        Seed::RenderCmd::SetClearColor({.2f, .2f, .2f, 1.0f});
        Seed::RenderCmd::Clear();

        Seed::Renderer2D::OpenScene(
            Seed::Scene{m_CameraCtrl.GetCamera()}); // look at me .... (make this took scene data...)

        Seed::Renderer2D::DrawQuad(m_Shaderlibrary.Get("texture"), m_textr, {13.5, 12.7}, {2, 3}, {.5, 0, 0},
                                   {3, 3, 0});
        Seed::Renderer2D::DrawQuad(m_Shaderlibrary.Get("texture"), m_textr2, TextCoord, {13, 18}, {-.3, 0, 0},
                                   {3, 3, 0});
        Seed::Renderer2D::CloseScene();
    };

    void OnImGuiDrawCall() override {
        // ImGui::Begin("tool-bar");
        // ImGui::DragFloat2("texture_coord", glm::value_ptr(TextCoord), 0.01, 0, 20);
        // ImGui::DragFloat2("texture_size", glm::value_ptr(TextSize), 0.01, 0, 20);
        // ImGui::End();
    };
};
