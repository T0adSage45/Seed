#include "materials.h"
#include <glm/ext/vector_float4.hpp>
#include <memory>

namespace Seed {

Materials::Materials(MaterialType mat_type, glm::vec4 m_color)
    : m_type(mat_type),
      m_flatColor(m_color) {

    const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 m_pos;
        layout(location = 1) in vec4 m_color;

        uniform mat4 u_ViewProjMatrix;
        uniform mat4 u_Transform;
        uniform vec4 u_Color;

        out vec4 v_Color;

        void main()
        {
        gl_Position = u_ViewProjMatrix * u_Transform  * vec4(m_pos.x, m_pos.y, m_pos.z, 1);
        v_Color = u_Color;
        } ;
    )";
    const char *fragmentShaderSource = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec4 v_Color;

        void main()
        {
        color = v_Color;
        };
    )";

    m_Shader.reset(Seed::Shader::Create(vertexShaderSource, fragmentShaderSource));
};

} // namespace Seed
