R"glsl(
#version 330 core

in vec2 TexCoord;
in vec3 FragColor;
out vec4 FragOutput;

uniform sampler2D texture1;

void main()
{
    FragOutput = texture(texture1, TexCoord) * vec4(FragColor, 1.0);
}
)glsl"