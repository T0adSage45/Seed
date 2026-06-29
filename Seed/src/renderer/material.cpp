#include "materials.h"

namespace Seed {

Materials *Materials::s_instance;

Materials::Materials(MaterialType mat_type)
    : m_type(mat_type) {

    const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 m_pos;
        layout(location = 1) in vec4 m_color;

        uniform mat4 u_ViewProjMatrix;
        uniform mat4 u_Transform;

        out vec3 v_Pos;
        out vec4 v_Color;

        void main()
        {
        gl_Position = u_ViewProjMatrix * u_Transform  * vec4(m_pos.x, m_pos.y, m_pos.z, 1);
        v_Color = m_color;
        } ;
    )";
    const char *fragmentShaderSource = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Pos;
        in vec4 v_Color;

        void main()
        {
        color = vec4(0.3f, 0.3f, 0.7f, 1.0f);
        color = v_Color;
        };
    )";

    m_Shader.reset(Seed::Shader::Create(vertexShaderSource, fragmentShaderSource));
};

} // namespace Seed
