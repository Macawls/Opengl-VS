R"glsl(
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

uniform sampler2D texture1;

void main()
{
	// Output color = color of the texture at the specified UV
	color = texture(texture1, UV);
}
)glsl"