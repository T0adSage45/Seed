#pragma once

#include <glm/ext/vector_float4.hpp>

namespace Seed {

enum class MaterialType { FlatShader, ToonShader, TextureShader };

class Materials {
public:
    Materials(MaterialType mat_type, glm::vec4 m_color = {1.0f, 1.0f, 1.0f, 1.0f});

    static Materials *Create(MaterialType type) { return new Materials(type); };

    void SetColor(const glm::vec4 &color) { m_flatColor = color; };
    glm::vec4 GetColor() const { return m_flatColor; };

    // std::shared_ptr<Shader> GetShader() { return m_Shader; }

private:
    MaterialType m_type;
    // std::shared_ptr<Shader> m_Shader;
    glm::vec4 m_flatColor;
};

} // namespace Seed
