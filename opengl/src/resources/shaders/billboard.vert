R"glsl(
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

out vec2 UV;

// Values that stay constant for the whole mesh.
uniform vec3 CamRightWorld;
uniform vec3 CamUpWorld;

uniform mat4 View;
uniform mat4 Projection;

uniform vec3 BillboardPos; 
uniform vec2 BillboardSize; 

void main()
{
	vec3 particleCenter_wordspace = BillboardPos;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CamRightWorld * aPos.x * BillboardSize.x
		+ CamUpWorld * aPos.y * BillboardSize.y;


	gl_Position = Projection * View * vec4(vertexPosition_worldspace, 1.0f);

	// Texture Mapping
	UV = aPos.xy + vec2(0.5, 0.5);
}
)glsl"