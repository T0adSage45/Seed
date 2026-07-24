#include "shader.h"
#include "log.h"
#include "render.h"
#include "opengl.h"
#include <memory>

namespace Seed {

Shader *Shader::Create(const std::string &name, const std::string &filepath) {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::None:
        Seed_Trace("not suppourted right now");
        break;
    case RendererAPI::API::OpenGl:
        return new Gl_Shader(name, filepath);
    case RendererAPI::API::Vulkan:
        Seed_Trace("not suppourted right now");
        break;
    }

    Seed_Trace("buffer Intialization issue");
    return nullptr;
};

void ShaderLib::Add(const std::shared_ptr<Shader> &shader) {
    auto &name = shader->GetName();
    m_Shaders[name] = shader;
};

std::shared_ptr<Shader> ShaderLib::Load(const std::string &filepath) {
    /// /seed/shaders/shader.glsl
    auto slashPos = filepath.find_last_of("/");
    auto dotPos = filepath.find_last_of(".");
    std::string ss = (dotPos != std::string::npos) ? filepath.substr(0, dotPos) : filepath;
    std::string filename = (slashPos == std::string::npos) ? ss : ss.substr(slashPos + 1);

    auto shade = Shader::Create(filename, filepath);
    // Add(shade);
    // return shade;
}

std::shared_ptr<Shader> ShaderLib::Load(const std::string &name, const std::string &filepath) {
    auto shade = Shader::Create(name, filepath);
    // Add(shade);
    // return shade;
};

std::shared_ptr<Shader> ShaderLib::Get(const std::string &name) { return m_Shaders[name]; };
} // namespace Seed
