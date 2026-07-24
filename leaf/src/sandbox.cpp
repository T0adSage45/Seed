#include "camera.h"
#include "imgui.h"
#include "shader.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <seed.h>

class Demo : public Seed::Layer {
private:
    std::shared_ptr<Seed::VertexArr> m_vertarr, m_SQva;
    std::shared_ptr<Seed::Materials> m_flatmat, m_toonmat, m_texturemat;
    std::shared_ptr<Seed::Shader> m_flatshade, m_toonshade, m_textureshade;
    std::shared_ptr<Seed::Texture> m_texture, m_texture1, m_texture2, m_texture3, m_texture4,
        m_texture5;
    Seed::OrhtoCamCtrl m_CameraCtrl;
    glm::vec3 transform_Pos;
    glm::vec4 texture_color{1.0f, 1.0f, 1.0f, 1.0f}, clear_color{0.1f, 0.1f, 0.1f, 1.0f},
        square_color{1.0f, 1.0f, 1.0f, 1.0f};

public:
    Demo()
        : Layer("demo"),
          m_CameraCtrl(1200.0f / 720.0f) {};
    ~Demo() {};

    void OnDetach() override {};

    void OnAttach() override {
        Seed_Trace("attached demo layer...");
        // vertx arr
        m_vertarr.reset(Seed::VertexArr::Create());

        // vertx buff
        float vert[9 * 3] = {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
                             0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, //
                             0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

        std::shared_ptr<Seed::VertexBuffer> m_vbuff;
        m_vbuff.reset(Seed::VertexBuffer::Create(vert, sizeof(vert)));

        m_vbuff->SetLayout({{Seed::ShaderDataType::Float3, "m_Position", false},
                            {Seed::ShaderDataType::Float4, "m_Color", false},
                            {Seed::ShaderDataType::Float2, "m_Uv", false}});

        m_vertarr->AddVertBuffer(m_vbuff);

        // index buff
        unsigned int indices[3] = {0, 1, 2};
        std::shared_ptr<Seed::IndexBuffer> m_ibuff;
        m_ibuff.reset(Seed::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_ibuff->Bind();

        m_vertarr->SetIndexBuffer(m_ibuff);

        // Materials
        m_flatmat.reset(Seed::Materials::Create(Seed::MaterialType::FlatShader));
        m_texturemat.reset(Seed::Materials::Create(Seed::MaterialType::TextureShader));
        m_toonmat.reset(Seed::Materials::Create(Seed::MaterialType::ToonShader));

        // Shader
        m_flatshade.reset(Seed::Shader::Create());
        m_toonshade.reset(Seed::Shader::Create("toon", "Seed/src/shader/toon.glsl"));
        m_textureshade.reset(Seed::Shader::Create("texture", "Seed/src/shader/texture.glsl"));

        // next one

        m_SQva.reset(Seed::VertexArr::Create());

        float sq_vert[9 * 4] = {
            -1.5f, -1.5f, 0.0f, texture_color.r, texture_color.g, texture_color.b, texture_color.a,
            0.0,
            0.0, //
            1.5f,  -1.5f, 0.0f, texture_color.r, texture_color.g, texture_color.b, texture_color.a,
            1.0,
            0.0, //
            1.5f,  1.5f,  0.0f, texture_color.r, texture_color.g, texture_color.b, texture_color.a,
            1.0f,
            1.0f, //
            -1.5f, 1.5f,  0.0f, texture_color.r, texture_color.g, texture_color.b, texture_color.a,
            0.0f,
            1.0f //
        };

        std::shared_ptr<Seed::VertexBuffer> SQvb;
        SQvb.reset(Seed::VertexBuffer::Create(sq_vert, sizeof(sq_vert)));
        SQvb->SetLayout({{Seed::ShaderDataType::Float3, "m_Position", false},
                         {Seed::ShaderDataType::Float4, "m_Color", false},
                         {Seed::ShaderDataType::Float2, "m_Uv", false}});

        m_SQva->AddVertBuffer(SQvb);

        unsigned int sqindices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Seed::IndexBuffer> m_sqibuff;
        m_sqibuff.reset(Seed::IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));
        m_sqibuff->Bind();

        m_SQva->SetIndexBuffer(m_sqibuff);

        //////////////////////
        /// Text
        //////////////////////
        m_texture.reset(Seed::Texture::Create("leaf/texture/checker.png"));
        m_texture1.reset(Seed::Texture::Create("leaf/texture/seed.png"));
        m_texture2.reset(Seed::Texture::Create("leaf/texture/leaf.png"));
        m_texture3.reset(Seed::Texture::Create("leaf/texture/kyomi.png"));
        m_texture4.reset(Seed::Texture::Create("leaf/texture/kyomi1.png"));
        m_texture5.reset(Seed::Texture::Create("leaf/texture/kyomi2.png"));
    };

    void OnEvent(Seed::Event &e) override { m_CameraCtrl.OnEvent(e); };

    void OnUpdate(Seed::Timestep delta) override {
        m_CameraCtrl.OnUpdate(delta);

        Seed::RenderCmd::SetClearColor(clear_color);
        Seed::RenderCmd::Clear();

        Seed::Renderer::OpenScene(Seed::Scene{m_CameraCtrl.GetCamera()});

        // update for bind values

        for (int j = 0; j < 9; j++) {
            for (int i = 0; i < 9; i++) {
                if ((i + j) % 2 == 0) {
                    m_toonmat->SetColor(texture_color);
                } else {
                    m_toonmat->SetColor(square_color);
                }
                Seed::Renderer::Submit(m_SQva, m_textureshade, m_texturemat,
                                       {(float)i * 1.0f, (float)j * 1.0f, 1.0f}, glm::vec3(0.32f),
                                       m_texture);
            };
        };

        // Submit(vertcs,maerial,tranform,scale)
        m_flatmat->SetColor(texture_color);
        Seed::Renderer::Submit(m_SQva, m_textureshade, m_flatmat, {-1.6, 0, 0}, glm::vec3(0.4f),
                               m_texture5);
        Seed::Renderer::Submit(m_SQva, m_textureshade, m_flatmat, {-3, 1.5, 0}, glm::vec3(0.4f),
                               m_texture3);
        Seed::Renderer::Submit(m_SQva, m_textureshade, m_flatmat, {-3, 0, 0}, glm::vec3(0.4f),
                               m_texture2);
        Seed::Renderer::Submit(m_SQva, m_textureshade, m_flatmat, {-5, 0, 0}, glm::vec3(0.4f),
                               m_texture4);
        Seed::Renderer::Submit(m_vertarr, m_flatshade, m_toonmat, transform_Pos, glm::vec3{1.8f},
                               m_texture5);

        Seed::Renderer::CloseScene();
    };

    void OnImGuiDrawCall() override {
        ImGui::Begin("tools...");
        ImGui::Text("Basic Controls");
        ImGui::ColorEdit4("clear color", glm::value_ptr(clear_color));
        ImGui::ColorEdit4("txture color", glm::value_ptr(texture_color));
        ImGui::ColorEdit4("square color", glm::value_ptr(square_color));
        ImGui::End();
    };
};

class Leaf : public Seed::Application {
public:
    Leaf() {
        Seed_Trace("Memory Allocated on the heap....");
        Demo *demo_layer = new Demo();
        PushLayer(demo_layer);
    };
    ~Leaf() { Seed_Warn("Destroyed the Allocated mem...."); };
};

Seed::Application *Seed::CreateApp() {
    Seed_Trace("application created....");
    return new Leaf();
};
