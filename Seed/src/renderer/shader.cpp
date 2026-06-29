#include "shader.h"
#include "render.h"
#include "opengl.h"

namespace Seed {

Shader *Shader::Create(const std::string &vertexSrc, const std::string &fragmentSrc) {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::None:
        Seed_Trace("not suppourted right now");
        break;
    case RendererAPI::API::OpenGl:
        return new Gl_Shader(vertexSrc, fragmentSrc);
    case RendererAPI::API::Vulkan:
        Seed_Trace("not suppourted right now");
        break;
    }

    Seed_Trace("buffer Intialization issue");
    return nullptr;
};

} // namespace Seed
