R"glsl(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec3 FragColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    // model to world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    FragColor = color;
    TexCoord = aTexCoord;
    // normal to world space
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // determine pos with mvp
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)glsl"