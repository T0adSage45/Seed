#include "render.h"
#include "camera.h"
#include "materials.h"
#include "renderer/buffer.h"
#include "renderer/opengl.h"
#include "shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
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

void RenderCmd::SetClearColor(const glm::vec4 color) { s_instance->SetClearColor(color); };
void RenderCmd::Clear() { s_instance->Clear(); }
void RenderCmd::Draw(const std::shared_ptr<VertexArr> &va) { s_instance->Draw(va); };

// TODO:: TEMP FIX
Renderer::SceneData *Renderer::m_Scene = new Renderer::SceneData;

void Renderer::OpenScene(const Scene &scene) {
    m_Scene->m_viewprojection_mat = scene.cam.GetVP_Mat();
};

void Renderer::Submit(const std::shared_ptr<VertexArr> &va,
                      const std::shared_ptr<Shader> &shader,
                      const std::shared_ptr<Materials> &mat,
                      glm::vec3 transform) {
    va->Bind();
    shader->Bind();
    std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform(
        "u_Transform", glm::translate(glm::mat4(1.0f), transform));
    std::dynamic_pointer_cast<Gl_Shader>(shader)->UploadUniform("u_ViewProjMatrix",
                                                                m_Scene->m_viewprojection_mat);
    RenderCmd::Draw(va);
};
void Renderer::Flush() {};
void Renderer::CloseScene() {};
} // namespace Seed
