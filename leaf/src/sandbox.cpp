#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <seed.h>

class Demo : public Seed::Layer {
private:
    std::shared_ptr<Seed::VertexArr> m_vertarr;
    std::shared_ptr<Seed::Shader> m_Shader;
    std::shared_ptr<Seed::Shader> m_blShader;
    std::shared_ptr<Seed::VertexArr> m_SQva;
    std::shared_ptr<Seed::Materials> m_flatmat;
    std::shared_ptr<Seed::Materials> m_toonmat;
    Seed::PerspectiveCam m_Camera;
    // Seed::OrthographicCam m_Camera;
    glm::vec3 cam_Pos;
    glm::vec3 cam_Rot;
    glm::vec3 transform_Pos;
    glm::vec4 texture_color{0.3f, 0.9f, 0.7f, 1.0f};
    glm::vec4 clear_color{0.15f, 0.00f, 0.15f, 1.0f};
    glm::vec4 square_color{1.0f, 0.5f, 0.2f, 1.0f};

public:
    Demo()
        : Layer("demo"),
          m_Camera(glm::radians(45.0f), 700.0f / 700.0f, 1.0f, 100.0f),
          // m_Camera(-5.0f, 5.0f, -5.0f, 5.0f),
          cam_Pos(0.0f, 0.0f, -5.0f) {};
    ~Demo() {};

    void OnDetach() override {};

    void OnAttach() override {
        Seed_Trace("attached demo layer...");
        // vertx arr
        m_vertarr.reset(Seed::VertexArr::Create());

        // vertx buff
        float vert[3 * 7] = {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, //
                             0.5f,  -0.5f, 0.0f, 0.4f, 0.8f, 1.2f, 0.5f, //
                             0.0f,  0.5f,  0.0f, 0.0f, 7.0f, 0.0f, 1.0f};

        std::shared_ptr<Seed::VertexBuffer> m_vbuff;
        m_vbuff.reset(Seed::VertexBuffer::Create(vert, sizeof(vert)));

        m_vbuff->SetLayout({{Seed::ShaderDataType::Float3, "m_Position", false},
                            {Seed::ShaderDataType::Float4, "m_Color", false}});

        m_vertarr->AddVertBuffer(m_vbuff);

        // index buff
        unsigned int indices[3] = {0, 1, 2};
        std::shared_ptr<Seed::IndexBuffer> m_ibuff;
        m_ibuff.reset(Seed::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_ibuff->Bind();

        m_vertarr->SetIndexBuffer(m_ibuff);

        m_flatmat.reset(Seed::Materials::Create(Seed::MaterialType::FlatShader));

        // next one

        m_SQva.reset(Seed::VertexArr::Create());

        float sq_vert[7 * 4] = {
            -1.5f,
            -1.5f,
            0.0f,
            texture_color.r,
            texture_color.g,
            texture_color.b,
            texture_color.a, //
            1.5f,
            -1.5f,
            0.0f,
            texture_color.r,
            texture_color.g,
            texture_color.b,
            texture_color.a, //
            1.5f,
            1.5f,
            0.0f,
            texture_color.r,
            texture_color.g,
            texture_color.b,
            texture_color.a, //
            -1.5f,
            1.5f,
            0.0f,
            texture_color.r,
            texture_color.g,
            texture_color.b,
            texture_color.a //
        };

        std::shared_ptr<Seed::VertexBuffer> SQvb;
        SQvb.reset(Seed::VertexBuffer::Create(sq_vert, sizeof(sq_vert)));
        SQvb->SetLayout({{Seed::ShaderDataType::Float3, "m_Position", false},
                         {Seed::ShaderDataType::Float4, "m_color", false}});

        m_SQva->AddVertBuffer(SQvb);

        unsigned int sqindices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Seed::IndexBuffer> m_sqibuff;
        m_sqibuff.reset(Seed::IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));
        m_sqibuff->Bind();

        m_SQva->SetIndexBuffer(m_sqibuff);
        m_toonmat.reset(Seed::Materials::Create(Seed::MaterialType::ToonShader));

        m_Camera.SetPosition(cam_Pos);
        m_Camera.RecalcViewMatrix();
    };

    void OnEvent(Seed::Event &e) override { (void)&e; };

    void OnUpdate(Seed::Timestep delta) override {
        float speed = 2.0f * delta.GetSeconds();
        cam_Pos = m_Camera.GetPosition();
        cam_Rot = m_Camera.GetRotation();

        if (Seed::InputManager::IsKeyPressed(Seed_KeyLeft)) {
            cam_Pos.x += speed;
            m_Camera.SetPosition(cam_Pos);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyRight)) {
            cam_Pos.x -= speed;
            m_Camera.SetPosition(cam_Pos);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyJ)) {
            transform_Pos.x += speed;
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyK)) {
            transform_Pos.x -= speed;
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyL)) {
            transform_Pos.y += speed;
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyM)) {
            transform_Pos.y -= speed;
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyUp)) {
            cam_Pos.y -= speed;
            m_Camera.SetPosition(cam_Pos);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyDown)) {
            cam_Pos.y += speed;
            m_Camera.SetPosition(cam_Pos);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyA)) {
            cam_Pos.z += speed;
            m_Camera.SetPosition(cam_Pos);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyQ)) {
            cam_Pos.z -= speed;
            m_Camera.SetPosition(cam_Pos);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyE)) {
            cam_Rot.x += speed;
            m_Camera.SetRotation(cam_Rot);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyR)) {
            cam_Rot.x -= speed;
            m_Camera.SetRotation(cam_Rot);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyD)) {
            cam_Rot.y += speed;
            m_Camera.SetRotation(cam_Rot);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyF)) {
            cam_Rot.y -= speed;
            m_Camera.SetRotation(cam_Rot);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyC)) {
            cam_Rot.z += speed;
            m_Camera.SetRotation(cam_Rot);
        } else if (Seed::InputManager::IsKeyPressed(Seed_KeyV)) {
            cam_Rot.z -= speed;
            m_Camera.SetRotation(cam_Rot);
        }

        m_Camera.RecalcViewMatrix();

        Seed::RenderCmd::SetClearColor(clear_color);
        Seed::RenderCmd::Clear();

        Seed::Renderer::OpenScene(Seed::Scene{m_Camera});

        // update for bind values

        for (int j = 0; j < 40; j++) {
            for (int i = 0; i < 40; i++) {
                if ((i + j) % 2 == 0) {
                    m_toonmat->SetColor(texture_color);
                } else {
                    m_toonmat->SetColor(square_color);
                }
                Seed::Renderer::Submit(m_SQva, m_toonmat, {(float)i * 0.4f, (float)j * 0.4f, 1.0f},
                                       glm::vec3(0.1f));
            };
        };

        m_flatmat->SetColor(texture_color);
        Seed::Renderer::Submit(m_vertarr, m_flatmat, transform_Pos);

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
