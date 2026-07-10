#include "render.h"
#include "camera.h"
#include "materials.h"
#include "renderer/buffer.h"
#include "renderer/opengl.h"
#include "renderer/texture.h"
#include "shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <memory>

namespace Seed {

RendererAPI::API RendererAPI::s_rendererAPI = API::OpenGl;
std::unique_ptr<RendererAPI> RenderCmd::s_instance = std::make_unique<Gl_RendererAPI>();

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
void RenderCmd::Draw(const std::shared_ptr<VertexArr> &va) { s_instance->Draw(va); };

// TODO:: TEMP FIX
Renderer::SceneData *Renderer::m_Scene = new Renderer::SceneData;

void Renderer::OpenScene(const Scene &scene) {
    m_Scene->m_viewprojection_mat = scene.cam.GetVP_Mat();
};

void Renderer::Init() { RenderCmd::Init(); };
void Renderer::Submit(const std::shared_ptr<VertexArr> &va, const std::shared_ptr<Materials> &mat,
                      glm::vec3 transform, glm::vec3 scale,
                      const std::shared_ptr<Texture> &texture) {
    va->Bind();
    std::shared_ptr<Shader> shade = mat->GetShader();
    shade->Bind();
    if (texture != nullptr) {
        int slot = 0;
        texture->Bind(slot);
        std::dynamic_pointer_cast<Gl_Shader>(shade)->UploadUniform("u_Texture", slot);
    }
    std::dynamic_pointer_cast<Gl_Shader>(shade)->UploadUniform(
        "u_Transform",
        glm::translate(glm::mat4(1.0f), transform) * glm::scale(glm::mat4(1.0f), scale));
    std::dynamic_pointer_cast<Gl_Shader>(shade)->UploadUniform(
        "u_ViewProjMatrix", Renderer::m_Scene->m_viewprojection_mat);
    std::dynamic_pointer_cast<Gl_Shader>(shade)->UploadUniform("u_Color", mat->GetColor());
    RenderCmd::Draw(va);
};

void Renderer::Flush() {};
void Renderer::CloseScene() {};
} // namespace Seed
