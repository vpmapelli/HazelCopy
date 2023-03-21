#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

out vec2 v_TexColors;

void main()
{
    v_TexColors = a_TexCoords;
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexColors;
uniform sampler2D u_Texture;

uniform vec4 u_Color;

void main()
{
    color = texture(u_Texture, v_TexColors * 10) * u_Color;
}