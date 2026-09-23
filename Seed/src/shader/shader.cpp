#include "shader.h"
#include "core.h"
#include "utility/utility.h"
#include "log.h"
#include "render.h"
#include "opengl.h"
#include <memory>
#include <string>

namespace Seed {

Seed::Ref<Shader> Shader::Create(const std::string &name, const std::string &filepath) {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::None:
        Seed_Trace("not suppourted right now");
        break;
    case RendererAPI::API::OpenGl:
        return std::make_shared<Gl_Shader>(name, filepath);
    case RendererAPI::API::Vulkan:
        Seed_Trace("not suppourted right now");
        break;
    }

    Seed_Trace("buffer Intialization issue");
    return nullptr;
};

void ShaderLib::Add(const Seed::Ref<Shader> &shader) {
    auto &name = shader->GetName();
    m_Shaders[name] = shader;
};

Seed::Ref<Shader> &ShaderLib::Load(const std::string &filepath) {
    std::string filename = "___default__";
    Asset_Name(filepath, filename);
    auto shade = Shader::Create(filename, filepath);
    Add(shade);
    return shade;
}

Seed::Ref<Shader> &ShaderLib::Load(const std::string &name, const std::string &filepath) {
    auto shade = Shader::Create(name, filepath);
    Add(shade);
    return shade;
};

Seed::Ref<Shader> &ShaderLib::Get(const std::string &name) {
    SEED_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "shader not found...");
    return m_Shaders[name];
};
} // namespace Seed
