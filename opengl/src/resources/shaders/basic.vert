R"glsl(
#version 330 core

layout(location = 0) in vec3 aPos;
out vec3 FragColor;

uniform mat4 mvp;
uniform vec3 color;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    FragColor = color; 
}
)glsl"