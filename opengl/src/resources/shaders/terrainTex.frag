R"glsl(
#version 330 core

in vec2 TexCoord;
out vec4 FragOutput;

in float Height;

uniform sampler2D texture1;

void main()
{
    float h = (Height + 16)/32.0f;
    FragOutput = texture(texture1, TexCoord) * vec4(h, h, h, 1.0);
}
)glsl"