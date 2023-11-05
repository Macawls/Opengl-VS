R"glsl(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragColor;
out vec2 TexCoord;

uniform mat4 mvp;
uniform vec3 color;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    FragColor = color; 
    TexCoord = aTexCoord;
}
)glsl"