#type vertex
#version 330 core
layout(location = 0) in vec3 m_Pos;
layout(location = 1) in vec4 m_Color;
layout(location = 2) in vec2 m_Uv;

uniform mat4 u_ViewProjMatrix;
uniform mat4 u_Transform;

out vec4 f_color;

void main()
{
    gl_Position = u_ViewProjMatrix * vec4(m_Pos.x, m_Pos.y, m_Pos.z, 1);
    f_color = vec4(0.9,.7,.9,1.0);
};

#type pixel
#version 330 core
layout(location = 0) out vec4 color;

in vec4 f_color;

void main()
{
    color = f_color;
};
