R"glsl(
#version 330 core
// VERTEX SHADER

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 FragColor; // Pass the color to the fragment shader

uniform mat4 mvp; // model view projection matrix

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    FragColor = aColor;
}
)glsl"