#include "materials.h"
#include <glm/ext/vector_float4.hpp>
#include <memory>

namespace Seed {

Materials::Materials(MaterialType mat_type, glm::vec4 m_color)
    : m_type(mat_type),
      m_flatColor(m_color) {

    const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 m_Pos;
        layout(location = 1) in vec4 m_Color;
        layout(location = 2) in vec2 m_Uv;

        uniform mat4 u_ViewProjMatrix;
        uniform mat4 u_Transform;
        uniform vec4 u_Color;

        out vec4 f_color;
        out vec2 f_uv;

        void main()
        {
            gl_Position = u_ViewProjMatrix * u_Transform  * vec4(m_Pos.x, m_Pos.y, m_Pos.z, 1);
            f_uv = m_Uv;
            f_color = m_Color * u_Color;
        } ;
    )";
    const char *fragmentShaderSource = R"(
        #version 330 core
        layout(location = 0) out vec4 color;

        in vec4 f_color;
        in vec2 f_uv;

        uniform sampler2D u_Texture;

        void main()
        {
            color = texture(u_Texture, f_uv);
        };
    )";

    m_Shader.reset(Seed::Shader::Create(vertexShaderSource, fragmentShaderSource));
};

} // namespace Seed
