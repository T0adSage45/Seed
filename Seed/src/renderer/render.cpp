#include "render.h"
#include "camera/camera.h"
#include "core.h"
#include "log.h"
#include "materials.h"
#include "buffers/buffer.h"
#include "renderer/opengl.h"
#include "renderer/texture.h"
#include "shader/shader.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <memory>

namespace Seed {

// TODO: not good implemntation;
RendererAPI::API RendererAPI::s_rendererAPI = API::OpenGl;
Seed::Scope<RendererAPI> RenderCmd::s_instance = std::make_unique<Gl_RendererAPI>();

RenderCmd::RenderCmd() {
    //     switch (RendererAPI::GetAPI()) {
    //     case Seed::RendererAPI::API::None:
    //         Seed_Trace("not suppourted right now");
    //         break;
    //     case Seed::RendererAPI::API::OpenGl:
    //         s_instance = new Gl_RendererAPI;
    //         break;
    //     case Seed::RendererAPI::API::Vulkan:
    //         Seed_Trace("not suppourted right now");
    //         break;
    //     };
};

void RenderCmd::Init() { s_instance->Init(); };
void RenderCmd::SetClearColor(const glm::vec4 color) { s_instance->SetClearColor(color); };
void RenderCmd::Clear() { s_instance->Clear(); }
void RenderCmd::Draw(const Seed::Ref<VertexArr> &va) { s_instance->Draw(va); };

// TODO:: TEMP FIX
Renderer::SceneData *Renderer::m_Scene = new Renderer::SceneData;

void Renderer::OpenScene(const Scene &scene) {
    // look at me .... (add check for scene data in assert format to avoid check ovehead);
    m_Scene->m_viewprojection_mat = scene.cam.GetVP_Mat();
};

void Renderer::Init() { RenderCmd::Init(); };

void Renderer::Submit(const Seed::Ref<VertexArr> &va, Seed::Ref<Shader> &shader, glm::vec3 transform, glm::vec3 scale,
                      const Seed::Ref<Texture> &texture) {

    SEED_ASSERT(va != nullptr, "no valid vertex buffer provided");
    va->Bind();

    if (shader == nullptr) {
        Seed_Warn("no valid shader found shifting to fallback shader");
        shader = Shader::Create();
    };
    shader->Bind();

    if (texture != nullptr) {
        int slot = 0;
        texture->Bind(slot);
        std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform("u_Texture", slot);
    }
    std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform(
        "u_Transform", glm::translate(glm::mat4(1.0f), transform) * glm::scale(glm::mat4(1.0f), scale));
    std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform("u_ViewProjMatrix",
                                                                Renderer::m_Scene->m_viewprojection_mat);
    std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform("u_Color", glm::vec4{1.0f});

    RenderCmd::Draw(va);

    if (texture != nullptr) {
        texture->Unbind();
    };
};

void Renderer::Flush() {};
void Renderer::CloseScene() {};

struct Renderer2D_Data {
    glm::mat4 m_viewprojection_mat;
    Seed::Ref<VertexArr> va;
};
static Renderer2D_Data *m_dataQuad_Render2D;

void Renderer2D::Init() {
    m_dataQuad_Render2D = new Renderer2D_Data();

    float vert[9 * 4] = {
        -0.1f, -0.1f, 0.0f, 1.f, 1.f, 1.f, 1.f, 0, 0, //
        0.1f,  -0.1f, 0.0f, 1.f, 1.f, 1.f, 1.f, 1, 0, //
        0.1f,  0.1f,  0.0f, 1.f, 1.f, 1.f, 1.f, 1, 1, //
        -0.1f, 0.1f,  0.0f, 1.f, 1.f, 1.f, 1.f, 0, 1  //
    };

    m_dataQuad_Render2D->va.reset(Seed::VertexArr::Create());

    Seed::Ref<Seed::VertexBuffer> m_vbuff;
    m_vbuff.reset(Seed::VertexBuffer::Create(vert, sizeof(vert)));

    m_vbuff->SetLayout({{Seed::ShaderDataType::Float3, "m_Position", false},
                        {Seed::ShaderDataType::Float4, "m_Color", false},
                        {Seed::ShaderDataType::Float2, "m_Uv", false}});

    m_dataQuad_Render2D->va->AddVertBuffer(m_vbuff);

    unsigned int indices[3 * 2] = {0, 1, 2, 2, 3, 0};
    Seed::Ref<Seed::IndexBuffer> m_ibuff;
    m_ibuff.reset(Seed::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_ibuff->Bind();

    m_dataQuad_Render2D->va->SetIndexBuffer(m_ibuff);
};

void Renderer2D::OpenScene(const Scene &scene) { //
    m_dataQuad_Render2D->m_viewprojection_mat = scene.cam.GetVP_Mat();
};

void Renderer2D::DrawQuad(Seed::Ref<Shader> &shader, const Seed::Ref<Texture> &texture, glm::vec2 TextCoord,
                          glm::vec2 size, glm::vec3 transform, glm::vec3 scale) {

    TextCoord = {(TextCoord.x * 32) / 441, (TextCoord.y * 32) / 566};

    SEED_ASSERT(m_dataQuad_Render2D->va != nullptr, "no valid vertex buffer provided");
    m_dataQuad_Render2D->va->Bind();

    if (shader == nullptr) {
        Seed_Warn("no valid shader found shifting to fallback shader");
        shader = Shader::Create();
    };
    shader->Bind();

    if (texture != nullptr) {
        int slot = 0;
        texture->Bind(slot);
        std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform("u_Texture", slot);
    }
    std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform(
        "u_Transform", glm::translate(glm::mat4(1.0f), transform) * glm::scale(glm::mat4(1.0f), scale));
    std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform("u_ViewProjMatrix",
                                                                m_dataQuad_Render2D->m_viewprojection_mat);
    std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform("u_TextCoord", TextCoord);
    // std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform("u_Color", mat->GetColor());

    RenderCmd::Draw(m_dataQuad_Render2D->va);

    if (texture != nullptr) {
        texture->Unbind();
    };
};

void Renderer2D::CloseScene() {};

} // namespace Seed
