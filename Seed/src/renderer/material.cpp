#include "log.h"
#include "materials.h"
#include <glm/ext/vector_float4.hpp>

namespace Seed {

Materials::Materials(MaterialType mat_type, glm::vec4 m_color)
    : m_type(mat_type),
      m_flatColor(m_color) {

    switch (mat_type) {
    case MaterialType::FlatShader:
        // m_Shader.reset(Seed::Shader::Create("Seed/src/shader/flat.glsl"));
        return;
    case MaterialType::ToonShader:
        // m_Shader.reset(Seed::Shader::Create("Seed/src/shader/toon.glsl"));
        return;
    case MaterialType::TextureShader:
        // m_Shader.reset(Seed::Shader::Create("Seed/src/shader/texture.glsl"));
        return;
    default:
        Seed_Warn("Shader with these mat doesnt exist...");
    };
};

} // namespace Seed
