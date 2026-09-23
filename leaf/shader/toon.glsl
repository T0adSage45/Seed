#type vertex
#version 330 core
layout(location = 0) in vec3 m_Pos;
layout(location = 1) in vec4 m_Color;
layout(location = 2) in vec2 m_Uv;

uniform vec2 u_TextCoord;
uniform mat4 u_ViewProjMatrix;
uniform mat4 u_Transform;

out vec4 f_color;
out vec2 f_uv;

void main()
{
    gl_Position = u_ViewProjMatrix * u_Transform * vec4(m_Pos.x, m_Pos.y, m_Pos.z, 1);
    f_uv = m_Uv + u_TextCoord;
    f_color = m_Color;
};

#type pixel
#version 330 core
layout(location = 0) out vec4 color;

in vec4 f_color;
in vec2 f_uv;

uniform sampler2D u_Texture;
uniform float u_Steps;

void main()
{
    vec4 t = texture(u_Texture, f_uv) * f_color;
    float steps = u_Steps > 0.0 ? u_Steps : 4.0;
    color = floor(t * steps) / steps;
    color.a = 1.0;
};
